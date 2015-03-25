import QtQuick 2.0
import Sailfish.Silica 1.0

import "cachemanager.js" as CacheManager

Page {
    Label {
        id: errorMsg
        visible: false
        font.pixelSize: Theme.fontSizeExtraSmall
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: content.height

        PullDownMenu {
            id: topMenu
            MenuItem {
                text: qsTr("About BikeMe")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("About.qml"))
                }
            }
            MenuItem {
                text: qsTr("Refresh cities list")
                onClicked: {
                    errorMsg.visible = false;
                    topMenu.busy = true
                    dataProvider.getContracts(true)
                }
            }
        }

        ViewPlaceholder {
            id: placeholder
            enabled: citiesBrowser.model.count == 0
            text: qsTr("Welcome to BikeMe!")
            hintText: qsTr("Pull down to load cities list")
        }

        Column {
            id: content
            width: parent.width

            PageHeader { id: header; title: qsTr("Cities") }

            Flow {
                id: citiesBrowser
                width: parent.width
                height: childrenRect.height

                property alias model : citiesList

                property int itemsPerRow: 4
                property string selectedCountry;

                Repeater {
                    model: citiesList
                    delegate: CitiesDelegate { parentWidth: citiesBrowser.width; itemsPerRow: itemsPerRow }
                }

                ListModel {
                    id: citiesList
                }

                onSelectedCountryChanged: {
                    refreshView();
                }

                function refreshView() {
                    var selectedIndex = -1;

                    citiesList.clear();
                    CacheManager.countries.forEach(function(countryObject, i) {
                        if (selectedCountry === countryObject.country) {
                            selectedIndex = i;
                        }
                        citiesList.append(countryObject);
                    });

                    if (selectedIndex != -1) {
                        selectedIndex += itemsPerRow - selectedIndex % itemsPerRow;

                        CacheManager.citiesByCountry[selectedCountry].forEach(function(city) {
                            citiesList.insert(selectedIndex++, city);
                        });
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }

    Component.onCompleted: {
        dataProvider.getContracts(false);
    }

    Connections {
        target: dataProvider
        onContractsUpdated: {
            var res = contracts;
            try {
                var cities = JSON.parse(res);
                CacheManager.setCities(cities);
                citiesBrowser.refreshView();
            }
            catch (e) {
                errorMsg.text = e.message;
                errorMsg.visible = true;
            }

            topMenu.busy = false;
        }
    }
}
