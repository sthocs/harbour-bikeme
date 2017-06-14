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

QT += xml positioning

DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\"

SOURCES += src/harbour-bikeme.cpp \
    src/configmanager.cpp \
    src/citydata.cpp \
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
    src/parser/canada/montrealparser.cpp \
    src/parser/north_america/nabsaparser.cpp

HEADERS += \
    src/configmanager.h \
    src/citydata.h \
    src/parser/bikedataparser.h \
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
    src/parser/canada/montrealparser.h \
    src/parser/north_america/nabsaparser.h

OTHER_FILES += harbour-bikeme.desktop \
    rpm/harbour-bikeme.spec \
    rpm/harbour-bikeme.yaml \
    qml/harbour-bikeme.qml \
    qml/cover/CoverPage.qml \
    qml/items/StationMarker.qml \
    qml/pages/CitiesBrowser.qml \
    qml/pages/CitiesDelegate.qml \
    qml/pages/CitiesList.qml \
    qml/pages/SecondPage.qml \
    qml/pages/Favourites.qml \
    qml/pages/InteractiveMap.qml \
    qml/pages/About.qml \
    qml/pages/Settings.qml \
    qml/pages/cachemanager.js \
    qml/pages/utils.js \
    qml/pages/db.js \
    translations/*.ts

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/harbour-bikeme-en.ts \
    translations/harbour-bikeme-fr.ts \
    translations/harbour-bikeme-es.ts \
    translations/harbour-bikeme-sv.ts \
    translations/harbour-bikeme-pl.ts \
    translations/harbour-bikeme-sl_SI.ts \
    translations/harbour-bikeme-ru.ts

data.files = data
data.path = /usr/share/$${TARGET}

images.files = images
images.path = /usr/share/$${TARGET}

INSTALLS += data images

DISTFILES += \
    qml/util/ProgressInfoBar.qml \
    rpm/harbour-bikeme.changes
