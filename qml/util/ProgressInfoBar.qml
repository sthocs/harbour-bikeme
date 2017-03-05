/**
 * Copyright (c) 2015 Damien Tardy-Panis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **/

import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    id: bar

    property alias label: progressBar.label
    property alias minimumValue: progressBar.minimumValue
    property alias maximumValue: progressBar.maximumValue
    property alias value: progressBar.value
    property bool open: false
    property int newCount: 0
    property int errorCount: 0

    color: Theme.rgba(Theme.secondaryHighlightColor, 0.9)
    width: parent.width
    height: progressBar.height + Theme.paddingSmall
    enabled: opacity != 0

    ProgressBar {
        id: progressBar
        width: parent.width - statusColumn.width - Theme.paddingSmall
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }
    }

    Column {
        id: statusColumn
        anchors {
            left: progressBar.right
            verticalCenter: parent.verticalCenter
        }
        spacing: Theme.paddingSmall
        visible: newCount > 0 || errorCount > 0
        width: visible ? Math.max(newItem.width, errorItem.width) : 0

        Behavior on width {
            NumberAnimation {}
        }

        Item {
            id: newItem
            height: newImage.height
            width: visible ? newImage.width + newLabel.width + Theme.paddingSmall : 0
            visible: newCount > 0

            Image {
                id: newImage
                source: "image://theme/icon-s-update"
                height: Theme.iconSizeSmall
                width: Theme.iconSizeSmall
                fillMode: Image.PreserveAspectFit
                smooth: true
                asynchronous: true
            }

            Label {
                id: newLabel
                anchors {
                    left: newImage.right
                    leftMargin: Theme.paddingSmall
                }
                text: qsTr("%n Cities", "", newCount)
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }

        Item {
            id: errorItem
            height: errorImage.height
            width: visible ? errorImage.width + errorLabel.width + Theme.paddingSmall : 0
            visible: errorCount > 0

            Image {
                id: errorImage
                source: "image://theme/icon-s-high-importance"
                height: Theme.iconSizeSmall
                width: Theme.iconSizeSmall
                fillMode: Image.PreserveAspectFit
                smooth: true
                asynchronous: true
            }

            Label {
                id: errorLabel
                anchors {
                    left: errorImage.right
                    leftMargin: Theme.paddingSmall
                }
                text: qsTr("%n Error(s)", "", errorCount)
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }
    }

    TouchBlocker {
        anchors.fill: parent
    }

    states: [
        State {
            name: "open";  when: open
            PropertyChanges { target: bar; opacity: 1.0; }
        },
        State {
            name: "closed";  when: !open
            PropertyChanges { target: bar; opacity: 0.0 }
        }
    ]

    transitions: [
        Transition {
            from: "closed"
            to: "open"
            NumberAnimation { property: "opacity" }
        },
        Transition {
            from: "open"
            to: "closed"
            SequentialAnimation {
                PropertyAnimation { duration: 2000 }
                NumberAnimation { property: "opacity" }
            }
        }
    ]
}
