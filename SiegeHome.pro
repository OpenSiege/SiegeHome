TEMPLATE = app
CONFIG += c++17

QT += core qml quick

DEFINES -= UNICODE

RESOURCES += SiegeHome.qrc

HEADERS += src/InstallationTab.hpp
SOURCES += src/main.cpp src/InstallationTab.cpp
