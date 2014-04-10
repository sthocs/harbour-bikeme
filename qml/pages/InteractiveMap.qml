import QtQuick 2.0
import Sailfish.Silica 1.0
import QtLocation 5.0
import QtPositioning 5.1
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
    property string selectedStationName: ""
    property int selectedStationNumber: 0
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
        height: parent.height
        width: parent.width

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

            zoomLevel: 9 /* does not work for now, always starts at 9 whatever the value */

            Component.onCompleted: {
                mapLoaded = true;
                map.zoomLevel += 6;
                cacheManager.downloadCarto(city);
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

    Connections {
        target: cacheManager
        onCartoChanged: {
            nbStations = JSCacheManager.saveStations(cacheManager.cartoJson);
            console.log("Number of stations: " + nbStations);
            printStations(true);
            stationLoadingLabel.visible = false;
        }
        onNetworkStatusUpdated: {
            stationLoadingLabel.text = connected ? "Loading stations..." : "Not connected";
        }

        onGotStationDetails: {
            station.clear();
            console.log("got station details: " + stationDetails);
            station.append(JSON.parse(stationDetails));
        }
    }

    ListModel {
        id: stations
    }

    ListModel {
        id: station
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
            coordinate: QtPositioning.coordinate(model.latitude, model.longitude)
            sourceItem: Image {
                source: (name !== selectedStationName) ? "../icons/velib_pin.png" : "../icons/velib_selected.png"
            }
            anchorPoint.x: 32
            anchorPoint.y: 64

            MapMouseArea {
                anchors.fill: parent
                onClicked: {
                    stationNameLabel.text = "Updating...";
                    selectedStationName = name;
                    selectedStationNumber = 0;
                    cacheManager.getStationDetails(city, number);
                }
            }
        }
    }

    function printStations(centerMap) {
        if (stations.count > 0 && stations.count === nbStations) {
            // all items are and will stay on map
            return;
        }
        stations.clear();
//        var start = new Date();

        var stationsArray = JSCacheManager.getStations();
        if (centerMap) {
            var avgLat = 0;
            var avgLng = 0;
            var nbTestStations = 0;
            stationsArray.forEach(function(station) {
                avgLat += station.latitude;
                avgLng += station.longitude;
                if (station.latitude === 0) {
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
                if (stationsArray[i].latitude < topLeft.latitude &&
                    stationsArray[i].latitude > bottomRight.latitude &&
                    stationsArray[i].longitude > topLeft.longitude &&
                    stationsArray[i].longitude < bottomRight.longitude) {
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
        anchors.leftMargin: 12
        MouseArea {
            anchors.fill: parent
            onClicked: {
                stopGeolocation();
                interactiveMap.destroy();
                pageStack.pop();
            }
        }
    }

    /* Add the selected station in favourites */
    Image {
        source: selectedStationNumber !== 0 ? "../icons/add_fav.png" : "../icons/add_fav_bw.png"
        anchors {
            left: infoColumn.right
            leftMargin: Theme.paddingLarge
            bottom: parent.bottom
            bottomMargin: Theme.paddingSmall
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
        id: radarIcon
        source: findMe ? "../icons/radar.png" : "../icons/radar_bw.png"
        anchors {
            right: parent.right
            rightMargin: Theme.paddingSmall
            bottom: parent.bottom
            bottomMargin: Theme.paddingSmall
        }
        sourceSize.height: Theme.iconSizeMedium * 9 / 10
        sourceSize.width: Theme.iconSizeMedium * 9 / 10
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
        source: positionReceived ? "../icons/center_position.png" : "../icons/center_position_bw.png"
        anchors {
            right: radarIcon.left
            rightMargin: Theme.paddingMedium
            bottom: parent.bottom
            bottomMargin: Theme.paddingSmall
        }
        sourceSize.height: (Theme.iconSizeSmall + Theme.iconSizeMedium) / 2
        sourceSize.width: (Theme.iconSizeSmall + Theme.iconSizeMedium) / 2
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

    Column {
        id: infoColumn
        anchors {
            bottom: parent.bottom
            bottomMargin: Theme.paddingSmall
            left: parent.left
            leftMargin: Theme.paddingSmall
        }
        spacing: Theme.paddingSmall

        Label {
            id: stationLoadingLabel
            visible: true
            text: "Loading stations..."
            color: "black"
        }

        Rectangle {
            color: "#FFFFFF"
            height: stationNameLabel.height
            width: interactiveMap.width * 54 / 100

            Label {
                id: stationNameLabel
                text: ""
                color: "black"
                font.pixelSize: Theme.fontSizeSmall
                font.capitalization: Font.Capitalize
            }
        }

        Rectangle {
            color: "#FFFFFF"
            height: stationNameLabel.height
            width: interactiveMap.width * 54 / 100

            Repeater {
                model: station
                delegate: Label {
                    text: "Bikes: " + available_bikes + " ; Parking: " + available_bike_stands
                    color: "black"
                    font.pixelSize: Theme.fontSizeMedium
                }
            }
        }

        Rectangle {
            color: "#FFFFFF"
            height: stationNameLabel.height
            width: interactiveMap.width * 54 / 100

            Repeater {
                model: station
                delegate: Label {
                    text: calcDate(last_update)
                    color: "black"
                    font.pixelSize: Theme.fontSizeSmall
                }
                onItemAdded: {
                    stationNameLabel.text = station.get(0).name.toLowerCase();
                    selectedStationNumber = station.get(0).number;
                }
            }
        }
    }


    function calcDate(updated) {
        var now = new Date();
        var elapsedSeconds = (now.getTime() - updated) / 1000;
        var min = Math.floor(elapsedSeconds / 60);
        var sec = Math.floor(elapsedSeconds % 60);
        return "Updated: " + min + " min " + sec + " sec"
    }
}
