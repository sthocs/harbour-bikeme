import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.bikeme 1.0

import "../util"

Page {
    property CitiesModel citiesModel

    Label {
        id: errorMsg
        text: citiesModel.errorMsg
        font.pixelSize: Theme.fontSizeExtraSmall
    }

    Component.onCompleted: {
        citiesModel.fetchFinished.connect(function() {
            topMenu.busy = false;
        });
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
            width: listView.width

            PageHeader { title: qsTr("Cities") }

            SearchField {
                id: searchField
                width: parent.width
                placeholderText: qsTr("Filter")
                visible: citiesModel.count > 0

                onTextChanged: citiesModelProxy.filter(text)

                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: focus = false
            }
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
            text: qsTr("Welcome to BikeMe!")
            hintText: qsTr("Pull down to load cities list")
        }

        PullDownMenu {
            id: topMenu
            MenuItem {
                text: qsTr("About BikeMe")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("About.qml"))
                }
            }
            MenuItem {
                text: qsTr("Update cities list")
                onClicked: {
                    topMenu.busy = true
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

            onClicked: {
                configManager.saveSetting("city", identifier);
                pageStack.push(Qt.resolvedUrl("SecondPage.qml"),
                               { city: citiesModel.cityAt(citiesModelProxy.realIndex(index)) });
            }
        }
    }

    ProgressInfoBar {
        id: progressInfoBar
        label: qsTr("Fetching operators...")
        open: topMenu.busy
        newCount: citiesModel.count
        errorCount: citiesModel.errorsCount
        minimumValue: 0
        maximumValue: citiesModel.providersCount
        value: citiesModel.fetchedProvidersCount

        anchors.bottom: parent.bottom
    }
}
