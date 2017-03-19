import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.bikeme 1.0

CoverBackground {
    property StationsFavouritesProxy favouritesModel

    Image {
        id: image
        anchors.centerIn: parent
        opacity: 0.1
        source: "../icons/cyclo.png"
    }
    Image {
        id: coverIcon
        anchors.centerIn: parent
        opacity: 0.9
        source: "../icons/bikeme_cover.png"
        visible: pageStack.currentPage.objectName !== "favourites"
    }
    Label {
        visible: !favouritesModel
        anchors.top: parent.top
        anchors.topMargin: Theme.paddingLarge
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Theme.fontSizeLarge
        anchors.left: cover.left
        anchors.right: cover.right

        text: pageStack.currentPage.city ?
                  pageStack.currentPage.city.name : "BikeMe"
    }
    ListView {
        id: favourites
        visible: pageStack.currentPage.objectName === "favourites"
        anchors.fill: parent
        anchors.bottomMargin: Theme.itemSizeMedium

        model: favouritesModel
        delegate: Column {
            width: parent.width

            Label {
                x: Theme.paddingSmall
                text: decodeURIComponent(model.name.toLowerCase())
                font.capitalization: Font.Capitalize
                font.pointSize: Theme.fontSizeExtraSmall
            }

            Row {
                x: Theme.paddingMedium
                width: parent.width

                Image {
                    source: "../icons/bikeme.png"
                    sourceSize.height: Theme.fontSizeSmall
                    sourceSize.width: Theme.fontSizeSmall
                    anchors.verticalCenter: parent.verticalCenter
                }
                Label {
                    text: " : " + (opened && available_bikes != -1 ?
                                       available_bikes : 0)
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
                    text: " : " + (opened && available_bike_stands != -1 ?
                                       available_bike_stands : 0)
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeMedium
                    font.bold: true
                }
            }
        }
    }

    CoverActionList {
        id: coverAction
        enabled: pageStack.currentPage.objectName === "favourites"

        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: {
                if (favouritesModel) {
                    favouritesModel.refreshAll();
                }
            }
        }
    }
}


