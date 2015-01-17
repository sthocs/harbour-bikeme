import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    Component.onCompleted: {
        cacheManager.getContracts(false);
    }

    Connections {
        target: cacheManager
        onContractsUpdated: {
            var res = contracts;
            try {
                var cities = JSON.parse(res);
                cities.forEach(function(city) {
                    // Temporary fix after API change.
                    if (city.countryCode) {
                        city.country_code = city.countryCode;
                    }
                    if (city.commercialName) {
                        city.commercial_name = city.commercialName;
                    }
                });
                cities.sort(function(a, b) {
                    return a.country_code.charCodeAt(0) - b.country_code.charCodeAt(0);
                });
                cityModel.clear();
                for (var i = 0; i < cities.length; ++i) {
                    cityModel.append(cities[i]);
                }
                errorMsg.visible = false;
            }
            catch (e) {
                errorMsg.text = e.message;
                errorMsg.visible = true;
            }

            topMenu.busy = false;
        }
    }

    Label {
        id: errorMsg
        visible: false
        font.pixelSize: Theme.fontSizeExtraSmall
    }

    ListModel {
        id: cityModel
    }

    SilicaListView {
        id: listView
        anchors.fill: parent

        model: cityModel

        header: PageHeader { title: "Cities" }

        section {
            property: 'country_code'

            delegate: Row {
                spacing: Theme.paddingMedium
                height: Theme.itemSizeExtraSmall
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingLarge

                Image {
                    source: "../icons/" + section + ".png"
                    sourceSize.height: Theme.iconSizeMedium * 2 / 3
                    sourceSize.width: Theme.iconSizeMedium
                }

                Label {
                    text: section
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.highlightColor
                }
            }
        }

        ViewPlaceholder {
            id: placeholder
            enabled: listView.count == 0
            text: "Welcome to BikeMe!"
            hintText: "Pull down to load cities list"
        }

        PullDownMenu {
            id: topMenu
            MenuItem {
                text: "About BikeMe"
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("About.qml"))
                }
            }
            MenuItem {
                text: "Refresh cities list"
                onClicked: {
                    topMenu.busy = true
                    cacheManager.getContracts(true)
                }
            }
        }

        VerticalScrollDecorator {}

        delegate:  ListItem {
            id: listItem

            Column {
                anchors.verticalCenter: parent.verticalCenter

                Label {
                    id: mainText
                    x: Theme.paddingLarge
                    text: model.name
                    font.weight: Font.Bold
                    font.pixelSize: Theme.fontSizeSmall
                }

                Label {
                    id: subText
                    x: Theme.paddingLarge
                    text: model.commercial_name
                    font.weight: Font.Light
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor

                    visible: text != ""
                }
            }

            onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"), { city: name });
        }
    }
}
