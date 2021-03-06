import QtQuick 2.2
import Sailfish.Silica 1.0

Item {
    id: tooltip
    width: Theme.itemSizeHuge * 1.5
    height: body.height + arrow.height / 2

    property bool opened: false
    property int bikes: -1
    property int electricBikes: -1
    property int parkings: -1
    property string last_update: "N/A"
    property string name: ""
    property string address: ""
    property bool favourited: false

    signal favouriteClicked()

    MouseArea {
        anchors.fill: body
    }

    Rectangle {
        id: body
        z: 2
        height: column.height + Theme.paddingMedium * 2
        width: parent.width
        color: "white"
        radius: 10

        Column {
            id: column
            anchors {
                left : parent.left
                right: parent.right
                top: parent.top
                margins: Theme.paddingMedium
            }
            spacing: Theme.paddingSmall
            Row {
                width: parent.width
                anchors.margins: Theme.paddingSmall
                Text {
                    width: parent.width
                    text: name
                    color: "black"
                    font.pixelSize: Theme.fontSizeTiny
                    wrapMode: Text.WordWrap
                }
            }
            Rectangle {
                height: Theme.paddingSmall
                width: parent.width
            }
            Row {
                Image {
                    source: "../icons/icon-bikeme.svg"
                    sourceSize.height: Theme.fontSizeSmall
                    sourceSize.width: Theme.fontSizeSmall
                }
                Label {
                    visible: opened
                    text: " : " + (bikes < 0 ? qsTr("Updating...") :
                                               qsTr("%n bike(s) available(s)", "", bikes))
                    color: "black"
                    font.pixelSize: Theme.fontSizeTiny
                }
            }
            Row {
                visible: electricBikes > -1
                width: parent.width
                anchors.margins: Theme.paddingSmall
                Image {
                    source: "../icons/icon-bikeme-elec.svg"
                    sourceSize.height: Theme.fontSizeSmall
                    sourceSize.width: Theme.fontSizeSmall
                }
                Text {
                    visible: opened
                    width: parent.width - Theme.fontSizeSmall
                    text: " : " + (electricBikes < 0 ? qsTr("Updating...") :
                                  qsTr("%n electric bike(s) available(s)", "", electricBikes))
                    color: "black"
                    font.pixelSize: Theme.fontSizeTiny
                    wrapMode: Text.WordWrap
                }
            }
            Row {
                anchors.margins: Theme.paddingSmall
                Image {
                    source: "../icons/parking.svg"
                    sourceSize.height: Theme.fontSizeSmall
                    sourceSize.width: Theme.fontSizeSmall
                    height: Theme.fontSizeSmall
                    width: Theme.fontSizeSmall
                }
                Label {
                    visible: opened
                    text: " : " + (parkings < 0 ? qsTr("Updating...") :
                                                  qsTr("%n parking(s) available(s)", "", parkings))
                    color: "black"
                    font.pixelSize: Theme.fontSizeTiny
                }
            }
            Row {
                width: parent.width
                anchors.margins: Theme.paddingSmall
                visible: last_update !== "N/A"
                Image {
                    height: Theme.fontSizeSmall
                    width: Theme.fontSizeSmall
                    source: "image://theme/icon-m-refresh?black"
                    fillMode: Image.PreserveAspectCrop
                }
                Text {
                    width: parent.width
                    text: " : " + last_update
                    color: "black"
                    font.pixelSize: Theme.fontSizeTiny
                    wrapMode: Text.WordWrap
                }
            }
            Row {
                width: parent.width
                anchors.margins: Theme.paddingSmall
                visible: address !== ""
                Image {
                    height: Theme.fontSizeSmall
                    width: Theme.fontSizeSmall
                    source: "image://theme/icon-m-location?black"
                    fillMode: Image.PreserveAspectCrop
                }
                Text {
                    width: parent.width - Theme.fontSizeSmall
                    text: " : " + address
                    color: "black"
                    font.pixelSize: Theme.fontSizeTiny
                    wrapMode: Text.WordWrap
                }
            }
            Rectangle {
                height: Theme.paddingSmall
                width: parent.width
            }
            Row {
                width: parent.width
                anchors.margins: Theme.paddingSmall

                Image {
                    id: favIcon
                    height: Theme.iconSizeSmall
                    width: Theme.iconSizeSmall
                    anchors.leftMargin: Theme.paddingMedium
                    source: favourited ? "image://theme/icon-m-favorite-selected?gold" : "image://theme/icon-m-favorite?black"
                    fillMode: Image.PreserveAspectCrop
                }
                Text {
                    width: parent.width - Theme.fontSizeSmall
                    height: Theme.iconSizeSmall
                    verticalAlignment: Text.AlignVCenter
                    text: '<a href="#">' + (favourited ? qsTr("Remove from favourites") : qsTr("Add to favourites")) + '</a>'
                    color: "black"
                    linkColor: "dodgerblue"
                    font.underline: true
                    font.bold: true
                    font.pixelSize: Theme.fontSizeTiny
                    wrapMode: Text.WordWrap
                    onLinkActivated: {
                        tooltip.favouriteClicked()
                        favourited = !favourited
                        if (favourited) {
                            starAnim.start()
                        }
                    }
                }
            }
        }
    }
    Rectangle {
        id: arrow
        x: body.width / 2 - width / 2
        y: body.height - (height * Math.sqrt(2) / 2) // rotated by 45 and moved up by half diagonal
                                                     // -> actual height will be height / 2
        width: Theme.iconSizeMedium; height: Theme.iconSizeMedium
        color: "white"
        rotation: 45
    }

    SequentialAnimation {
        id: starAnim
        SequentialAnimation {
            YAnimator {
                target: favIcon;
                from: 0
                to: -15
                easing.type: Easing.OutExpo;
                duration: 200
            }
            YAnimator {
                target: favIcon;
                from: -15
                to: 0
                easing.type: Easing.OutBounce;
                duration: 800
            }
        }
    }
}
