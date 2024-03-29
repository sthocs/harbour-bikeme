import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.bikeme 1.0

/* Themes values on Jolla:
 * itemSizeExtraSmall=70; itemSizeSmall=80; itemSizeMedium=100
 * iconSizeSmall=32; iconSizeMedium=64; iconSizeLarge=96
 */

Page {
    property City city

    Component.onCompleted: {
        window.cover.countryCode = city.countryCode
    }
    Component.onDestruction: {
        window.cover.countryCode = ""
    }

    SilicaListView {
        id: listView
        anchors.fill: parent

        PullDownMenu {
            id: topMenu
            MenuItem {
                text: qsTr("Settings")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("Settings.qml"));
                }
            }
        }

        model: ListModel {
            id: favouritesModel

            ListElement {
                name: qsTr("Stations Map")
                fileName: "InteractiveMap"
            }
            ListElement {
                name: qsTr("Favourites")
                fileName: "Favourites"
            }
        }

        header: PageHeader {
            title: city.name
            description: city.commercialName
        }
        delegate: BackgroundItem {
            id: delegate

            Label {
                x: Theme.paddingLarge
                anchors.verticalCenter: parent.verticalCenter
                text: name
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
            onClicked: {
                pageStack.push(Qt.resolvedUrl(fileName + ".qml"), { city : city });
            }
        }
    }

    Label {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: Theme.paddingMedium

        font.pixelSize: Theme.fontSizeExtraSmall
        color: Theme.secondaryColor
        text: city.copyright
    }
}
