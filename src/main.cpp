
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <iostream>
#include <windows.h>

#include "InstallationTab.hpp"

bool isUserAdmin();

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
    
    // if we're not running elevated then lets disable the write registry key button and change the label
    if (!isUserAdmin())
    {
        QObject* writeRegistryKeysButton = engine.rootObjects().first()->findChild<QObject*>("writeRegistryKeysButton", Qt::FindChildrenRecursively);
        writeRegistryKeysButton->setProperty("text", "Run as Administrator to set Registry Keys");
        writeRegistryKeysButton->setProperty("enabled", false);
    }

    return app.exec();
}

// function to check if the program is running with elevated privileges
bool isUserAdmin()
{
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;
    BOOL b = AllocateAndInitializeSid (&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &AdministratorsGroup);

    if (b)
    {
        if (!CheckTokenMembership(NULL, AdministratorsGroup, &b))
        {
            b = FALSE;
        }

        FreeSid (AdministratorsGroup);
    }

    return b;
}