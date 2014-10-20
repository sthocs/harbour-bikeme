import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    width: Theme.iconSizeSmall // 32
    height: Theme.iconSizeSmall
    property int available: -1
    property bool selected

    property string _COLOR_AVAILABLE : "#77A834"
    property string _COLOR_ALMOST_EMPTY : "orange"
    property string _COLOR_EMPTY : "red"
    property string _COLOR_SELECTED1 : "steelblue" // blue when data is available
    property string _COLOR_SELECTED2 : "#D60080" // pink when no data is available

    Rectangle {
        z: 2
        width: Theme.iconSizeSmall; height: Theme.iconSizeSmall
        radius: 10
        color: selected ? (available >= 0 ? _COLOR_SELECTED1 : "white") :
                          available < 0 ? "white" :
                                          available === 0 ? _COLOR_EMPTY :
                                                            available <= 3 ? _COLOR_ALMOST_EMPTY : _COLOR_AVAILABLE
        border.color: available >= 0 ? "black" : selected ? _COLOR_SELECTED2 : _COLOR_AVAILABLE
        border.width: available < 0 ? 3 : 1

        Label {
            visible: available != -1
            text: available
            font.pixelSize: Theme.fontSizeMedium
            color: "black"
            font.weight: Font.Bold
            anchors.centerIn: parent
        }
        Image {
            visible: available == -1
            anchors.centerIn: parent
            source: "../icons/cyclo_mini.png"
            sourceSize.height: Theme.iconSizeSmall * 0.75
            sourceSize.width: Theme.iconSizeSmall * 0.75
        }
    }
    Rectangle {
        x: Theme.iconSizeSmall / 4; y: Theme.iconSizeSmall / 4 * 2.5
        width: Theme.iconSizeSmall / 2; height: Theme.iconSizeSmall / 2 // c²=512 -> c=22.63 -> c/2=11.31
        color: selected ? (available >= 0 ? _COLOR_SELECTED1 : _COLOR_SELECTED2) :
                          available < 0 ? _COLOR_AVAILABLE :
                                          available === 0 ? _COLOR_EMPTY :
                                                            available <= 3 ? _COLOR_ALMOST_EMPTY : _COLOR_AVAILABLE
        border.color: "black"
        rotation: 45
    }
}
