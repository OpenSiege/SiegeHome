
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <iostream>

#include "InstallationTab.hpp"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<InstallationTab>("SiegeHome.InstallationTab", 1, 0, "InstallationTab");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:main.qml")));

    InstallationTab installTabBackend;
    installTabBackend.setEngine(&engine);
    engine.rootContext()->setContextProperty("installTabBackend", &installTabBackend);

    installTabBackend.reloadPaths();

    return app.exec();
}