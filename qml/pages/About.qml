import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    SilicaFlickable {
        anchors.fill: parent
        anchors.topMargin: Theme.paddingMedium

        contentWidth: parent.width;
        contentHeight: header.height + contentColumn.height + Theme.paddingLarge

        PageHeader {
            id: header
            anchors.topMargin: Theme.paddingMedium

            Flow {
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingMedium

                Label {
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.highlightColor
                    text: qsTr("About")
                }
                Item {
                    width: Theme.paddingSmall
                    height: parent.height
                }
                Text { text: "B"; color: "#FEBA02"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
                Text { text: "i"; color: "#028CBB"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
                Text { text: "k"; color: "#ea2c81"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
                Text { text: "e"; color: "#c0bc01"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
                Text { text: "M"; color: "#76a1d6"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
                Text { text: "e"; color: "#c491c5"; font.pixelSize: Theme.fontSizeExtraLarge; font.bold: true }
            }
        }

        Column {
            id: contentColumn
            anchors {
                left: parent.left
                right: parent.right
                top: header.bottom
                leftMargin: Theme.paddingMedium
                rightMargin: Theme.paddingMedium
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "../../images/banner.png"
                width: parent.width
                fillMode: Image.PreserveAspectFit
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
                text: qsTr("Find a bike in your city!")
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
                text: qsTr("This app helps you to find a bike or a free parking \
spot for various self-service bicycle schemes arround the world:\
<ul>%1<li>And more!</li></ul>\
BikeMe provides a simple interactive map displaying all stations by city, their number of \
available bikes and free parking spots, and the geolocation.<br />\
Click on %2 to activate the geolocation and on %3 \
to center the map on your position.<br/>\
Regular users: Save your favourites stations and see their status directly on the cover!")
.arg("<li>JCDecaux</li><li>Nextbike</li><li>Smoove</li>")
.arg('<img src="image://theme/icon-m-gps" width="' + Theme.iconSizeSmall + '" height="' + Theme.iconSizeSmall + '"/>')
.arg('<img src="image://theme/icon-cover-location" width="' + Theme.iconSizeSmall + '" height="' + Theme.iconSizeSmall + '"/>')
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
                text: qsTr("Version %1").arg(Qt.application.version)
            }

            Label {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                text: "2014 - 2017 Sthocs"
            }

            Item {
                width: parent.width
                height: 2 * Theme.paddingLarge
            }

            Text {
                width: parent.width
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
                text: qsTr("Many thanks for the translations: %1").arg("ZB, eson, atlochowski");
            }
        }
    }
}
