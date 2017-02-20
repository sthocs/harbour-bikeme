import QtQuick 2.0
import Sailfish.Silica 1.0

import com.jolla.harbour.bikeme 1.0

/* Themes values on Jolla:
 * itemSizeExtraSmall=70; itemSizeSmall=80; itemSizeMedium=100
 * iconSizeSmall=32; iconSizeMedium=64; iconSizeLarge=96
 */

Page {
    id: secondPage

    property City city

    Component.onCompleted: {
        configManager.saveSetting("city", city.name);
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
                name: "Interactive Map"
                fileName: "InteractiveMap2"
            }
            ListElement {
                name: "Favourites"
                fileName: "Favourites2"
            }
        }

        header: PageHeader {
            title: city.name
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
        text: dataProvider.getCopyright(city.name)
    }
}
