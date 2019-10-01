cmake_minimum_required(VERSION 3.1.0)

project(SiegeHome)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Qml Quick Gui Widgets)
find_package(Qt5QuickCompiler)

qt5_add_resources(RESOURCES SiegeHome.qrc)

add_executable(SiegeHome src/main.cpp src/InstallationTab.cpp ${RESOURCES})
target_link_libraries(SiegeHome Qt5::Core Qt5::Quick Qt5::Widgets)
target_compile_features(SiegeHome PUBLIC cxx_std_17)
