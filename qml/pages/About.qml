import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.paddingMedium
        anchors.rightMargin: Theme.paddingMedium

        PageHeader {}

        Item {
            width: parent.width
            height: Theme.paddingLarge
        }

        Row {
            id: title
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: titleField
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.primaryColor
                text: "About "
            }
            Text { text: " B"; color: "#FEBA02"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
            Text { text: "i"; color: "#028CBB"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
            Text { text: "k"; color: "#ea2c81"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
            Text { text: "e"; color: "#c0bc01"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
            Text { text: "M"; color: "#76a1d6"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
            Text { text: "e"; color: "#c491c5"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
        }

        Item {
            width: parent.width
            height: Theme.paddingLarge
        }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            source: "../icons/bikeme.png"
        }

        Item {
            width: parent.width
            height: Theme.paddingLarge
        }

        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Theme.fontSizeMedium
            color: Theme.primaryColor
            text: "Find a bike in your city!"
        }
        Item {
            width: parent.width
            height: Theme.paddingLarge
        }
        Text {
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.primaryColor
            width: parent.width
            wrapMode: Text.WordWrap
            linkColor: Theme.highlightColor
            onLinkActivated: Qt.openUrlExternally("http://en.cyclocity.com")
            text: 'This app helps you to find a bike or a free parking spot for various self-service bicycle ' +
                  'schemes arround the world:' +
                  '<ul><li>JCDecaux</li><li>Nextbike</li><li>Smoove</li><li>And more!</li></ul>' +
                  'It provides a simple interactive map displaying all stations by city, their number of ' +
                  'available bikes and free parking spots, and the geolocation.<br />' +
                  ' Click on <img src="image://theme/icon-m-gps" width="' + Theme.iconSizeSmall + '" height="' +
                  Theme.iconSizeSmall + '"/> to activate the geolocation and on ' +
                  '<img src="image://theme/icon-cover-location" width="' + Theme.iconSizeSmall + '" height="' +
                  Theme.iconSizeSmall + '"/> to center the map on your position.<br/>' +
                  'Regular users: Save your favourites stations and see their status directly on the cover!'
        }

        Item {
            width: parent.width
            height: 2 * Theme.paddingLarge
        }

        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            text: "Version 0.6.beta"
        }

        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            text: "Copyright © 2014 - 2017 Sthocs"
        }
    }
}
