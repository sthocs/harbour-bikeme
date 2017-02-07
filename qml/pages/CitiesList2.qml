import QtQuick 2.0
import Sailfish.Silica 1.0

import com.jolla.harbour.bikeme 1.0

Page {
    Label {
        id: errorMsg
        visible: false
        font.pixelSize: Theme.fontSizeExtraSmall
    }

    CitiesModel {
        id: citiesModel
    }
    CitiesModelProxy {
        id: citiesModelProxy
        sourceModel: citiesModel
    }

    SilicaListView {
        id: listView
        anchors.fill: parent

        model: citiesModelProxy
        currentIndex: -1

        header: Column {
            width: parent.width

            PageHeader { title: "Cities2" }
        }

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
                text: qsTr("Display: List")
                onClicked: {
                    configManager.saveSetting("citiesDisplay", "browser");
                    pageStack.replace(Qt.resolvedUrl("CitiesBrowser.qml"))
                }
            }
            MenuItem {
                text: "Refresh cities list"
                onClicked: {
                    //topMenu.busy = true
                    citiesModel.loadAll()
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
