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

QT += xml

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
    src/parser/russia/parsermoscow.cpp

OTHER_FILES += qml/harbour-bikeme.qml \
    qml/cover/CoverPage.qml \
    qml/items/StationMarker.qml \
    qml/pages/CitiesList.qml \
    qml/pages/SecondPage.qml \
    qml/pages/Favourites.qml \
    qml/pages/InteractiveMap.qml \
    qml/pages/About.qml \
    qml/pages/Settings.qml \
    qml/pages/cachemanager.js \
    rpm/harbour-bikeme.spec \
    rpm/harbour-bikeme.yaml \
    harbour-bikeme.desktop

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
    src/parser/russia/parsermoscow.h
