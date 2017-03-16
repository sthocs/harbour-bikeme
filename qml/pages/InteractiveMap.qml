import QtQuick 2.0
import Sailfish.Silica 1.0
import QtLocation 5.0
import QtPositioning 5.1
import harbour.bikeme 1.0

import "../items"
import "./db.js" as Db
import "./utils.js" as Utils


Page {
    id: interactiveMap
    width: parent.width
    height: parent.height
    backNavigation: false;

    // Indicates if the user has requested to display his position
    property bool findMe: false
    property bool positionReceived: false

    property City city
    property int selectedStationNumber: 0
    property bool isSelectedStationInFav: false
    property bool displayAllStatus: configManager.getSetting("displayAllStatus") !== "false"
    property bool displayAvailableParking: false

    property string mapPlugin: getMapPlugin()
    property int maxItemsOnMap: 200


    //! Used to indicate if device is in portrait mode
    property bool isPortrait: (screen.currentOrientation === Screen.Landscape) ? false : true

    //! We stop retrieving position information when component is to be destroyed
    Component.onDestruction: positionSource.stop();


    //! Container for map element
    Rectangle {
        id : mapview
        anchors.fill: parent
        anchors.bottomMargin: Theme.itemSizeMedium

        //! Map element centered with current position
        Map {
            id: map
            z: 1
            plugin : Plugin {
                name : mapPlugin;

                parameters: [
                    PluginParameter { name: "app_id"; value: "" },
                    PluginParameter { name: "app_code"; value: "" }
                ]
            }
            anchors.fill: parent
            // Paris, Hotel de Ville. Doesn't actually work (for now?)
            center: QtPositioning.coordinate(48.85604723, 2.35390723)

            zoomLevel: 9 /* does not work for now (Qt5.1), always starts at 9 whatever the value */

            Component.onCompleted: {
                center = QtPositioning.coordinate(43.5508823, 7.0168207);
                map.zoomLevel = city.zoom || 13;
                updateFilter();
                stationLoadingLabel.visible = true;
                stations.city = city;
                stations.loadStationsList();
            }

            gesture.onFlickFinished: {
                console.log("Flick FINISHED");
            }
            gesture.onPanFinished: {
                console.log("Pan FINISHED");
                updateFilter();
            }
            gesture.onPinchFinished: {
                console.log("Pinch FINISHED");
                updateFilter();
            }

            //! Icon to display the current position
            MapQuickItem {
                id: meIcon
                z: 2
                coordinate: positionSource.position.coordinate
                visible: positionReceived
                sourceItem: Image {
                    source: "../icons/me.png"
                }
                anchorPoint.x: 24
                anchorPoint.y: 24
            }

            MapMouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    map.center = map.toCoordinate(Qt.point(mouseX, mouseY));
                    map.zoomLevel = (map.zoomLevel + 1) < map.maximumZoomLevel ? map.zoomLevel + 1 :
                                                                                 map.maximumZoomLevel;
                    updateFilter();
                }
            }
        }
    }

    Rectangle {
        id: alertPopup
        visible: false
        anchors.top: parent.top
        anchors.right: parent.right
        height: Theme.itemSizeSmall
        width: parent.width * 2 / 3
        color: "gray"
        opacity: 0.8
        radius: 7

        Label {
            id: alertMsg
            text: "Mode not available"
            color: "black"
        }

        Image {
            source: "image://theme/icon-m-close"
            anchors.right: parent.right

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    alertPopup.visible = false;
                }
            }
        }
    }

    StationsModel {
        id: stations

        onCenterChanged: {
            map.center = center;
            updateFilter();
        }
        onStationsLoaded: {
            stationLoadingLabel.visible = false;
            refreshLabel.visible = false;
            stationsProxy.applyFilter = count > maxItemsOnMap;
            if (!withDetails && displayAllStatus) {
                if (interactiveMap.city.isAllStationModeSupported()) {
                    refreshAll();
                }
                else {
                    displayAllStatus = false;
                    alertMsg.text = "Can't display all status\nfor this city.";
                    alertPopup.visible = true;
                }
            }
        }
        onStationUpdated: {
            if (station.number === selectedStationNumber) {
                stationNameLabel.text = station.name;
                numberOfBikes.text = ": " + station.available_bikes;
                numberOfParking.text = ": " + station.available_bike_stands;
                lastUpdatedTime.text = "Updated: " + Utils.makeLastUpdateDateHumanReadable(station.last_update);
            }
        }
        onError: {
            stationLoadingLabel.visible = false;
            refreshLabel.text = errorMsg;
            refreshLabel.visible = true;
            stationNameLabel.text = "Error";
        }
    }

    StationsModelProxy {
        id: stationsProxy
        sourceModel: stations
    }

    Repeater {
        id: stationsRepeater
        onItemAdded: {
            if (map.mapItems.length < maxItemsOnMap)
                map.addMapItem(item)
        }
        onItemRemoved: {
            map.removeMapItem(item)
        }
        parent: map
        model: stationsProxy
        delegate: MapQuickItem {
            coordinate: model.coordinates
            sourceItem: StationMarker {
                displayAllStatus: interactiveMap.displayAllStatus
                opened: model.opened
                available: displayAvailableParking ? model.available_bike_stands : model.available_bikes
                selected: number != 0 && number === selectedStationNumber
            }

            anchorPoint.x: Theme.iconSizeSmall / 2 // 16
            anchorPoint.y: Theme.iconSizeSmall / 4 * 5 // 40

            MapMouseArea {
                anchors.fill: parent
                onClicked: {
                    selectedStationNumber = number;
                    isSelectedStationInFav = Db.isFavourite(city.identifier, number);
                    if (!displayAllStatus) {
                        refreshLabel.visible = false;
                        if (!city.isSingleStationModeSupported()) {
                            alertMsg.text = "Only \"all status\" mode\navailable for this city."
                            alertPopup.visible = true;
                            displayAllStatus = true;
                            refreshAll();
                        }
                        stationNameLabel.text = "Updating...";
                        numberOfBikes.text = ":";
                        numberOfParking.text = ":";
                        lastUpdatedTime.text = "Updated:"
                        stations.fetchStationInformation(stationsProxy.sourceRow(index));
                    }
                }
            }
        }
    }


    //! Source for retrieving the positioning information
    PositionSource {
        id: positionSource

        //! Desired interval between updates in milliseconds
        updateInterval: 10000
        active: false

        onPositionChanged: {
            if (!positionReceived) { // First time we receive a position, go to it.
                console.log("GPS position received");
                positionReceived = true;
                // Keep coordinates in a different object to avoid map auto-centering.
                var pos = QtPositioning.coordinate(positionSource.position.coordinate.latitude,
                                                   positionSource.position.coordinate.longitude);
                map.center = pos;
                updateFilter();
            }
        }
    }

    function startGeolocation() {
        findMe = true;
        positionSource.start();
    }

    function stopGeolocation() {
        findMe = false;
        positionSource.stop();
    }

    Image {
        source: "../icons/icon-m-back.png"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: Theme.paddingMedium
        MouseArea {
            anchors.fill: parent
            onClicked: {
                stopGeolocation();
                interactiveMap.destroy();
                pageStack.pop();
            }
        }
    }

    /* Button to add the selected station in favourites */
    Image {
        source: isSelectedStationInFav ? "image://theme/icon-m-favorite-selected" : "image://theme/icon-m-favorite"
        anchors {
            right: centerPosIcon.left
            rightMargin: Theme.paddingLarge
            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium
        }
        sourceSize.height: Theme.iconSizeMedium
        sourceSize.width: Theme.iconSizeMedium

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (selectedStationNumber !== 0) {
                    if (isSelectedStationInFav) {
                        Db.removeFavourite(city.identifier, selectedStationNumber);
                    }
                    else {
                        Db.addFavourite(city.identifier, selectedStationNumber);
                    }
                    isSelectedStationInFav = !isSelectedStationInFav;
                }
            }
        }
    }

    Image {
        id: gpsIcon
        source: findMe ? "image://theme/icon-m-gps?" + Theme.highlightColor : "image://theme/icon-m-gps"
        anchors {
            right: parent.right
            rightMargin: Theme.paddingMedium
            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium
        }
        sourceSize.height: Theme.iconSizeMedium
        sourceSize.width: Theme.iconSizeMedium
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (!findMe) {
                    startGeolocation();
                }
                else {
                    stopGeolocation();
                }
            }
        }
    }

    Image {
        id: centerPosIcon
        source: positionReceived ? "image://theme/icon-cover-location" : "image://theme/icon-cover-location?gray"
        anchors {
            right: gpsIcon.left
            rightMargin: Theme.paddingMedium
            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium
        }
        sourceSize.height: Theme.iconSizeMedium
        sourceSize.width: Theme.iconSizeMedium
        visible: findMe

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (positionReceived) {
                    map.center = map.toCoordinate(Qt.point(map.width / 2, map.height / 2));
                    var pos = QtPositioning.coordinate(positionSource.position.coordinate.latitude,
                                                       positionSource.position.coordinate.longitude)
                    map.center = pos;
                    updateFilter();
                }
            }
        }
    }

    Label {
        id: stationLoadingLabel
        visible: true
        text: "Loading stations..."
        color: "black"
        anchors.bottom: mapview.bottom
    }

    Column {
        visible: !displayAllStatus
        anchors {
            top: mapview.bottom
            bottom: parent.bottom
            bottomMargin: Theme.paddingSmall
            left: parent.left
            leftMargin: Theme.paddingSmall
        }

        Label {
            id: stationNameLabel
            text: "-"
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
            font.capitalization: Font.Capitalize
        }
        Row {
            Image {
                source: "../icons/bikeme.png"
                sourceSize.height: Theme.fontSizeSmall
                sourceSize.width: Theme.fontSizeSmall
                anchors.verticalCenter: parent.verticalCenter
            }
            Label {
                id: numberOfBikes
                text: ":"
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeMedium
                font.bold: true
            }
            Item {
                width: Theme.paddingLarge
                height: 1
            }
            Image {
                source: "../icons/parking.png"
                sourceSize.height: Theme.fontSizeSmall
                sourceSize.width: Theme.fontSizeSmall
                anchors.leftMargin: Theme.paddingSmall
                anchors.verticalCenter: parent.verticalCenter
            }
            Label {
                id: numberOfParking
                text: ":"
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeMedium
                font.bold: true
            }
        }
        Label {
            id: lastUpdatedTime
            text: "Updated:"
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }

    Label {
        id: refreshLabel
        color: "black"
        visible: false
        anchors.left: parent.left
        anchors.bottom: rightControls.top
        anchors.leftMargin: Theme.paddingMedium
        anchors.bottomMargin: Theme.paddingMedium
    }
    Row {
        id: rightControls
        visible: displayAllStatus
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: Theme.paddingMedium
        anchors.bottomMargin: Theme.paddingMedium
        spacing: Theme.paddingSmall

        Image {
            source: displayAvailableParking ? "../icons/bikeme_bw.png" : "../icons/bikeme.png"
            sourceSize.height: Theme.itemSizeSmall
            sourceSize.width: Theme.itemSizeSmall
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    displayAvailableParking = false;
                }
            }
        }
        Image {
            source: displayAvailableParking ? "../icons/parking.png" : "../icons/parking_bw.png"
            sourceSize.height: Theme.itemSizeSmall
            sourceSize.width: Theme.itemSizeSmall
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    displayAvailableParking = true;
                }
            }
        }
        Image {
            source: "image://theme/icon-m-refresh"
            sourceSize.height: Theme.itemSizeSmall
            sourceSize.width: Theme.itemSizeSmall
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    refreshAll();
                }
            }
        }
    }


    function updateFilter() {
        stationsProxy.filter(map.toCoordinate(Qt.point(0, 0)),
                             map.toCoordinate(Qt.point(map.width, map.height)));
        // This is a bit hacky: we limit the number of station on the maps, so some stations
        // in the filter are not displayed. If the number of stations in the filter decrease
        // (e.g. because of a zoom), we could now display them, but the onItemAdded signal is
        // not triggered since those stations were already in the filter. So refreshing manually
        if (map.mapItems.length < maxItemsOnMap && stationsRepeater.count > map.mapItems.length) {
            map.clearMapItems();
            map.addMapItem(meIcon);
            for (var i = 0; i < stationsRepeater.count && i < maxItemsOnMap; ++i) {
                map.addMapItem(stationsRepeater.itemAt(i));
            }
        }
    }

    function refreshAll() {
        refreshLabel.text = qsTr("Refreshing...");
        refreshLabel.visible = true
        stations.loadAllStationsDetails();
    }

    function getMapPlugin() {
        var supportedProviders = [ "here", "osm" ];
        var savedProvider = configManager.getSetting("mapProvider");
        if (supportedProviders.indexOf(savedProvider) >= 0) {
            return savedProvider;
        }
        return supportedProviders[0];
    }
}
