import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    width: Theme.iconSizeSmall // 32
    height: Theme.iconSizeSmall
    property bool displayAllStatus: false
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
        color: {
            if (!displayAllStatus || available < 0) {
                return "white";
            }
            else if (selected) {
                return _COLOR_SELECTED1;
            }
            else {
                if (available === 0) return _COLOR_EMPTY
                else if (available <= 3) return _COLOR_ALMOST_EMPTY
                else return _COLOR_AVAILABLE
            }
        }

        border.color: (available >= 0 && displayAllStatus) ? "black" : selected ? _COLOR_SELECTED2 : _COLOR_AVAILABLE
        border.width: (available < 0 || !displayAllStatus) ? 3 : 1

        Label {
            visible: displayAllStatus && available != -1
            text: available
            font.pixelSize: Theme.fontSizeMedium
            color: "black"
            font.weight: Font.Bold
            anchors.centerIn: parent
        }
        Image {
            visible: !displayAllStatus || available == -1
            anchors.centerIn: parent
            source: "../icons/cyclo_mini.png"
            sourceSize.height: Theme.iconSizeSmall * 0.75
            sourceSize.width: Theme.iconSizeSmall * 0.75
        }
    }
    Rectangle {
        x: Theme.iconSizeSmall / 4; y: Theme.iconSizeSmall / 4 * 2.5
        width: Theme.iconSizeSmall / 2; height: Theme.iconSizeSmall / 2 // c²=512 -> c=22.63 -> c/2=11.31
        color: {
            if (selected) {
                if (displayAllStatus) return _COLOR_SELECTED1;
                else return _COLOR_SELECTED2
            }
            else {
                if (!displayAllStatus || available < 0 || available > 3) return _COLOR_AVAILABLE;
                else if (available === 0) return _COLOR_EMPTY;
                else if (available <= 3) return _COLOR_ALMOST_EMPTY;
            }
        }

        border.color: "black"
        rotation: 45
    }
}
