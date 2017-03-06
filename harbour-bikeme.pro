# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-bikeme

CONFIG += sailfishapp

QT += xml \
positioning

SOURCES += src/harbour-bikeme.cpp \
    src/configmanager.cpp \
    src/citydata.cpp \
    src/dataprovider.cpp \
    src/staticdataprovider.cpp \
    src/parser/bikedataparserfactory.cpp \
    src/parser/france/jcdecauxparser.cpp \
    src/parser/france/parserveloway.cpp \
    src/parser/france/parserlille.cpp \
    src/parser/uk/parserlondon.cpp \
    src/parser/russia/parsermoscow.cpp \
    src/parser/france/parsersmoove.cpp \
    src/parser/kazakhstan/parserastana.cpp \
    src/citiesmodel.cpp \
    src/city.cpp \
    src/citiesmodelproxy.cpp \
    src/citiesloader.cpp \
    src/station.cpp \
    src/stationsloader.cpp \
    src/stationsmodel.cpp \
    src/stationsmodelproxy.cpp \
    src/stationsfavouritesproxy.cpp \
    src/parser/parsersregistry.cpp \
    src/parser/germany/nextbikeparser.cpp \
    src/parser/canada/montrealparser.cpp

OTHER_FILES += harbour-bikeme.desktop \
    rpm/harbour-bikeme.spec \
    rpm/harbour-bikeme.yaml \
    qml/harbour-bikeme.qml \
    qml/cover/CoverPage.qml \
    qml/items/StationMarker.qml \
    qml/pages/CitiesBrowser.qml \
    qml/pages/CitiesDelegate.qml \
    qml/pages/CitiesList.qml \
    qml/pages/CitiesList2.qml \
    qml/pages/SecondPage.qml \
    qml/pages/Favourites.qml \
    qml/pages/InteractiveMap.qml \
    qml/pages/About.qml \
    qml/pages/Settings.qml \
    qml/pages/cachemanager.js \
    qml/pages/utils.js \
    qml/pages/db.js

data.files = data
data.path = /usr/share/$${TARGET}

INSTALLS += \
    data

HEADERS += \
    src/configmanager.h \
    src/citydata.h \
    src/dataprovider.h \
    src/staticdataprovider.h \
    src/parser/bikedataparser.h \
    src/parser/bikedataparserfactory.h \
    src/parser/france/jcdecauxparser.h \
    src/parser/france/parserveloway.h \
    src/parser/france/parserlille.h \
    src/parser/uk/parserlondon.h \
    src/parser/russia/parsermoscow.h \
    src/parser/france/parsersmoove.h \
    src/parser/kazakhstan/parserastana.h \
    src/citiesmodel.h \
    src/city.h \
    src/citiesmodelproxy.h \
    src/citiesloader.h \
    src/station.h \
    src/stationsloader.h \
    src/stationsmodel.h \
    src/stationsmodelproxy.h \
    src/stationsfavouritesproxy.h \
    src/parser/parsersregistry.h \
    src/parser/germany/nextbikeparser.h \
    src/parser/canada/montrealparser.h

DISTFILES += \
    qml/pages/InteractiveMap2.qml \
    qml/pages/SecondPage2.qml \
    qml/pages/Favourites2.qml \
    qml/util/ProgressInfoBar.qml
