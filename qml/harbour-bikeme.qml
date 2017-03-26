import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.bikeme 1.0
import "cover"
import "pages"

ApplicationWindow
{
    id: window

    cover: CoverPage {}

    CitiesModel {
        id: citiesModel
    }

    Component.onCompleted: {
        var PAGES = ["default", "pages/InteractiveMap.qml", "pages/Favourites.qml"];

        var firstPage = {
            page: Qt.resolvedUrl("pages/CitiesList.qml"),
            properties: { citiesModel: citiesModel }
        };

        var city = citiesModel.get(configManager.getSetting("city"));
        if (city) {
            console.log("Restoring app state: " + city.name);
            var pagesToPush = [firstPage];
            pagesToPush.push({
                page: Qt.resolvedUrl("pages/SecondPage.qml"),
                properties: { city: city }
            });
            var startPage = configManager.getSetting("startPage");
            if (startPage > 0) {
                pagesToPush.push({
                    page: Qt.resolvedUrl(PAGES[startPage]),
                    properties: { city: city }
                });
            }

            pageStack.push(pagesToPush);
        }
        else {
            // Default page
            pageStack.push(firstPage.page, firstPage.properties);
        }
    }
}


