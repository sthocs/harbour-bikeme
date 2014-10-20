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

SOURCES += src/harbour-bikeme.cpp \
    src/configmanager.cpp \
    src/cachemanager.cpp

OTHER_FILES += qml/harbour-bikeme.qml \
    qml/cover/CoverPage.qml \
    qml/pages/CitiesList.qml \
    qml/pages/SecondPage.qml \
    qml/pages/Favourites.qml \
    qml/pages/InteractiveMap.qml \
    qml/pages/About.qml \
    qml/pages/cachemanager.js \
    rpm/harbour-bikeme.spec \
    rpm/harbour-bikeme.yaml \
    harbour-bikeme.desktop \
    qml/items/StationMarker.qml \
    qml/pages/Settings.qml

HEADERS += \
    src/configmanager.h \
    src/cachemanager.h

