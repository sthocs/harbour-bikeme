import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    width: childrenRect.width
    height: childrenRect.height

    property int parentWidth
    property int itemsPerRow : 4

    Loader {
        sourceComponent: {
            if (country) {
                return countrySelector;
            }
            return cityRow;
        }
    }

    Component {
        id: countrySelector

        Rectangle {
            width: parentWidth / itemsPerRow
            height: Theme.itemSizeLarge
            color: citiesBrowser.selectedCountry === country ? "transparent" : Theme.highlightDimmerColor

            Item {
                height: childrenRect.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                Image {
                    id: flag
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "../icons/" + country + ".png"
                    sourceSize.height: Theme.iconSizeMedium * 2 / 3
                    sourceSize.width: Theme.iconSizeMedium
                }
                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: flag.bottom
                    text: country
                    color: Theme.primaryColor
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    citiesBrowser.selectedCountry =
                            citiesBrowser.selectedCountry === country ? "" : country;
                }
            }
        }
    }

    Component {
        id: cityRow

        BackgroundItem {
            width: parentWidth
            height: cityName.height + commercialName.height + Theme.paddingSmall

            Column {
                Label {
                    id: cityName
                    x: Theme.paddingLarge
                    text: name
                    font.weight: Font.Bold
                    font.pixelSize: Theme.fontSizeSmall
                }
                Label {
                    id: commercialName
                    x: Theme.paddingLarge
                    text: model.commercial_name
                    font.weight: Font.Light
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                }
            }

            onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"), { city: name });
        }
    }
}
