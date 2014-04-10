import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: cover

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
        anchors.top: parent.top
        anchors.topMargin: Theme.paddingLarge
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Theme.fontSizeLarge
        anchors.left: cover.left
        anchors.right: cover.right

        text: pageStack.currentPage.city || "BikeMe"
    }
    Label {
        id: label
        anchors.centerIn: parent
        font.pixelSize: Theme.fontSizeSmall
        text: {
            if (pageStack.currentPage.objectName === "favourites") {
                return pageStack.currentPage.nbRefreshingStations !== 0 ?
                            "Refreshing...\n" + pageStack.currentPage.coverLabel :
                            pageStack.currentPage.coverLabel;
            }
            else {
                return "";
            }
        }
        visible: pageStack.currentPage.objectName === "favourites"
    }

    CoverActionList {
        id: coverAction
        enabled: pageStack.currentPage.objectName === "favourites"

        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: {
                if (pageStack.currentPage.objectName === "favourites") {
                    pageStack.currentPage.refreshAll();
                }
            }
        }
    }
}


