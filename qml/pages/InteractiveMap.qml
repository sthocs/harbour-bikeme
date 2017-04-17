import QtQuick 2.0
import Sailfish.Silica 1.0
import QtLocation 5.0
import QtPositioning 5.1
import QtGraphicalEffects 1.0
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
                if (mapPlugin === "osm") {
                    map.zoomLevel += 1;
                }
                updateFilter();
                stationLoadingLabel.visible = true;
                stations.city = city;
                stations.loadStationsList();
            }

            gesture.onPanFinished: {
                console.log("Pan FINISHED");
                updateFilter();
            }
            gesture.onPinchFinished: {
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
        height: Theme.itemSizeMedium
        width: parent.width * 2 / 3
        color: "gray"
        opacity: 0.8
        radius: 7

        Text {
            id: alertMsg
            text: "Mode not available"
            color: "black"
            font.pixelSize: Theme.fontSizeSmall
            wrapMode: Text.Wrap
            anchors.fill: parent
            anchors.leftMargin: Theme.paddingSmall
            anchors.rightMargin: closeAlertButton.width
            verticalAlignment: Text.AlignVCenter
        }

        Image {
            id: closeAlertButton
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
                    alertMsg.text = qsTr("Can't display all status for this city.");
                    alertPopup.visible = true;
                }
            }
        }
        onStationUpdated: {
            if (station.number === selectedStationNumber) {
                stationNameLabel.text = station.name;
                numberOfBikes.text = ": " + station.available_bikes;
                numberOfParking.text = ": " + station.available_bike_stands;
                lastUpdatedTime.text = qsTr("Updated: %1").arg(Utils.makeLastUpdateDateHumanReadable(station.last_update));
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
                            alertMsg.text = qsTr("Only \"all status\" mode available for this city.")
                            alertPopup.visible = true;
                            displayAllStatus = true;
                            refreshAll();
                        }
                        stationNameLabel.text = qsTr("Updating...");
                        numberOfBikes.text = ":";
                        numberOfParking.text = ":";
                        lastUpdatedTime.text = qsTr("Updated:")
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

    Item {
        width: Theme.iconSizeSmallPlus
        height: Theme.iconSizeMedium
        anchors.left: parent.left
        anchors.top: parent.top
        Image {
            source: "image://theme/icon-cover-next?DodgerBlue"
            mirror: true
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: Theme.paddingSmall
            anchors.topMargin: Theme.paddingMedium
        }
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
        text: qsTr("Loading stations...")
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
        }
        Row {
            Image {
                source: "../icons/icon-bikeme.svg"
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
                source: "../icons/parking.svg"
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
            text: qsTr("Updated:")
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

        Item {
            width: Theme.itemSizeSmall
            height: Theme.itemSizeSmall

            Image {
                id: displayAvailableBikesBtn
                source: "../icons/icon-bikeme.svg"
                sourceSize.height: Theme.itemSizeSmall
                sourceSize.width: Theme.itemSizeSmall
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        displayAvailableParking = false;
                    }
                }

            }
            Colorize {
                visible: displayAvailableParking
                anchors.fill: displayAvailableBikesBtn
                source: displayAvailableBikesBtn
                hue: 0
                saturation: 0
                lightness: 0
            }
        }
        Item {
            width: Theme.itemSizeSmall
            height: Theme.itemSizeSmall

            Image {
                id: displayAvailablePlacesBtn
                source: "../icons/parking.svg"
                sourceSize.height: Theme.itemSizeSmall
                sourceSize.width: Theme.itemSizeSmall
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        displayAvailableParking = true;
                    }
                }
            }
            Colorize {
                visible: !displayAvailableParking
                anchors.fill: displayAvailablePlacesBtn
                source: displayAvailablePlacesBtn
                hue: 0
                saturation: 0
                lightness: 0
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
