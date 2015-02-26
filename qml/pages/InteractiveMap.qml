import QtQuick 2.0
import Sailfish.Silica 1.0
import QtLocation 5.0
import QtPositioning 5.1
import "../items"
import "cachemanager.js" as JSCacheManager


Page {
    id: interactiveMap
    width: parent.width
    height: parent.height
    backNavigation: false;

    property bool mapLoaded: false
    // Indicates if the user has requested to display his position
    property bool findMe: false
    property bool positionReceived: false

    property string city: "Paris"
    property int selectedStationNumber: 0
    property bool displayAllStatus: true
    property bool displayAvailableParking: false

    property string mapPlugin: "nokia"
    property int maxItemsOnMap: 200
	property int nbStations


    //! Used to indicate if device is in portrait mode
    property bool isPortrait: (screen.currentOrientation === Screen.Landscape) ? false : true

    //! We stop retrieving position information when component is to be destroyed
    Component.onDestruction: positionSource.stop();

    onIsPortraitChanged: {
        if (mapLoaded) {
            printStations()
        }
    }

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
                    PluginParameter { name: "token"; value: "" }
                ]
            }
            anchors.fill: parent
            // Paris, Hotel de Ville by default. Updated after stations loading.
            center: QtPositioning.coordinate(48.856047, 2.353907)

            zoomLevel: 9 /* does not work for now (Qt5.1), always starts at 9 whatever the value */

            Component.onCompleted: {
                mapLoaded = true;
                map.zoomLevel += 6;
                dataProvider.downloadCarto(city);
            }

            gesture.onFlickFinished: {
                console.log("Flick FINISHED");
            }
            gesture.onPanFinished: {
                console.log("Pan FINISHED");
                printStations();
            }
            gesture.onPinchFinished: {
                console.log("Pinch FINISHED");
                printStations();
            }

            //! Icon to display the current position
            MapQuickItem {
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
                    printStations();
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

    Connections {
        target: dataProvider
        onCartoChanged: {
            nbStations = JSCacheManager.saveStations(dataProvider.cartoJson);
            console.log("Number of stations: " + nbStations);
            printStations(true);
            stationLoadingLabel.visible = false;
            if (displayAllStatus) {
                refreshLabel.visible = true;
                dataProvider.downloadAllStationsDetails(city);
            }
        }
        onNetworkStatusUpdated: {
            stationLoadingLabel.text = connected ? "Loading stations..." : "Not connected";
        }

        onGotStationDetails: {
            console.log("got station details: " + stationDetails);
            var stationDetailsJSON = JSON.parse(stationDetails);
            if (stationDetailsJSON.name) {
                stationNameLabel.text = stationDetailsJSON.name.toLowerCase();
            }
            else if (stationDetailsJSON.address) {
                stationNameLabel.text = stationDetailsJSON.address.toLowerCase();
            }
            numberOfBikes.text = ": " + stationDetailsJSON.available_bikes;
            numberOfParking.text = ": " + stationDetailsJSON.available_bike_stands;
            lastUpdatedTime.text = calcDate(stationDetailsJSON.last_update);
        }
        onGotAllStationsDetails: {
            var res = allStationsDetails;
            try {
                nbStations = JSCacheManager.saveStationsWithDetails(res);
                console.log("Got all stations details; Number of stations: " + nbStations);
                printStations(false, true);
                stationLoadingLabel.visible = false;
                refreshLabel.visible = false;
            }
            catch(e) {
                refreshLabel.text = res;
            }
        }
        onModeNotSupported: {
            if (displayAllStatus) {
                alertMsg.text = "Can't display all status\nfor this city."
                refreshLabel.visible = false;
            }
            else {
                alertMsg.text = "Only \"all status\" mode\navailable for this city."
                refreshLabel.visible = true;
                dataProvider.downloadAllStationsDetails(city);
            }
            displayAllStatus = !displayAllStatus;
            alertPopup.visible = true;
        }
    }

    ListModel {
        id: stations
    }

    Repeater {
        onItemAdded: {
            map.addMapItem(item)
        }
        onItemRemoved: {
            map.removeMapItem(item)
        }
        parent: map
        model: stations
        delegate: MapQuickItem {
            coordinate: QtPositioning.coordinate(model.position.lat, model.position.lng)
            sourceItem: StationMarker {
                available: displayAvailableParking ? model.available_bike_stands : model.available_bikes
                selected: number === selectedStationNumber
            }

            anchorPoint.x: Theme.iconSizeSmall / 2 // 16
            anchorPoint.y: Theme.iconSizeSmall / 4 * 5 // 40

            MapMouseArea {
                anchors.fill: parent
                onClicked: {
                    selectedStationNumber = number;
                    if (!displayAllStatus) {
                        stationNameLabel.text = "Updating...";
                        dataProvider.getStationDetails(city, number);
                    }
                }
            }
        }
    }

    function printStations(centerMap, forceRefresh) {
        if (!forceRefresh && stations.count > 0 && stations.count === nbStations) {
            // all items are and will stay on map
            return;
        }
        stations.clear();
//        var start = new Date();

        var stationsArray = JSCacheManager.getStations();
        if (!stationsArray) {
            return;
        }
        if (centerMap) {
            var avgLat = 0;
            var avgLng = 0;
            var nbTestStations = 0;
            stationsArray.forEach(function(station) {
                avgLat += station.position.lat;
                avgLng += station.position.lng;
                if (station.position.lat === 0) {
                    ++nbTestStations;
                }
            });
            avgLat /= (stationsArray.length - nbTestStations);
            avgLng /= (stationsArray.length - nbTestStations);
            map.center = QtPositioning.coordinate(avgLat, avgLng);
        }
//        var now = new Date();
//        console.log('Elapsed time: ' + (now.getTime() - start.getTime()));

        if (stationsArray.length < maxItemsOnMap) {
            stationsArray.forEach(function(station) {
                stations.append(station);
            });
        }
        else {
            var nbItemsOnMap = 0;
            var topLeft = map.toCoordinate(Qt.point(0, 0));
            var bottomRight = map.toCoordinate(Qt.point(map.width, map.height));
            console.log("TopLeft: " + topLeft.latitude + "/" + topLeft.longitude + " bottomRight: "+
                        bottomRight.latitude + "/" + bottomRight.longitude);
            for (var i = 0; i < stationsArray.length; ++i) {
                if (stationsArray[i].position.lat < topLeft.latitude &&
                    stationsArray[i].position.lat > bottomRight.latitude &&
                    stationsArray[i].position.lng > topLeft.longitude &&
                    stationsArray[i].position.lng < bottomRight.longitude) {
                    stations.append(stationsArray[i]);
                    if (++nbItemsOnMap > maxItemsOnMap) {
                        break;
                    }
                }
            };
        }
//        now = new Date();
//        console.log('Elapsed time: ' + (now.getTime() - start.getTime()));
    }

    //! Source for retrieving the positioning information
    PositionSource {
        id: positionSource

        //! Desired interval between updates in milliseconds
        updateInterval: 10000
        active: false

        //! When position changed, update the location strings
        onPositionChanged: {
            if (!positionReceived) { // First time we receive a position, go to it.
                console.log("GPS position received");
                positionReceived = true;
                // Keep coordinates in a different object to avoid map auto-centering.
                var pos = QtPositioning.coordinate(positionSource.position.coordinate.latitude,
                                                   positionSource.position.coordinate.longitude);
                map.center = pos;
                printStations();
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
        source: selectedStationNumber !== 0 ? "../icons/add_fav.png" : "../icons/add_fav_bw.png"
        anchors {
            right: centerPosIcon.left
            rightMargin: Theme.paddingLarge
            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium
        }
        sourceSize.height: (Theme.iconSizeSmall + Theme.iconSizeMedium) / 2 /* =48px on Jolla */
        sourceSize.width: (Theme.iconSizeSmall + Theme.iconSizeMedium) / 2

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (selectedStationNumber !== 0) {
                    pageStack.push(Qt.resolvedUrl("Favourites.qml"),
                                   { city: city, favouriteToAdd: selectedStationNumber });
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
                    printStations();
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
        text: "Refreshing..."
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
                    refreshLabel.text = qsTr("Refreshing...");
                    refreshLabel.visible = true
                    dataProvider.downloadAllStationsDetails(city);
                }
            }
        }
    }


    function calcDate(updated) {
        if (typeof updated === "string") {
            return "Updated: " + updated;
        }
        var now = new Date();
        var elapsedSeconds = (now.getTime() - updated) / 1000;
        var min = Math.floor(elapsedSeconds / 60);
        var sec = Math.floor(elapsedSeconds % 60);
        return "Updated: " + min + " min " + sec + " sec"
    }
}
