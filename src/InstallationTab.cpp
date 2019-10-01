
#include "InstallationTab.hpp"
#include <QQmlApplicationEngine>
#include <iostream>

// TODO: Decide whether to include Linux support?
#include <Windows.h>
#include <shlobj.h>

// This is all the possible spots that our registry keys should be located at.
constexpr const char* registryKeyVec[] =
{
    "Software\\Wow6432Node\\Microsoft\\Microsoft Games\\Dungeon Siege Legends of Aranna\\1.0",
    "Software\\Wow6432Node\\Microsoft\\Microsoft Games\\DungeonSiege\\1.0",
    "Software\\Wow6432Node\\Microsoft\\Microsoft Games\\DungeonSiegeDemo\\1.0",
    "Software\\Microsoft\\Microsoft Games\\Dungeon Siege Legends of Aranna\\1.0",
    "Software\\Microsoft\\Microsoft Games\\DungeonSiege\\1.0",
    "Software\\Microsoft\\Microsoft Games\\DungeonSiegeDemo\\1.0"
};

InstallationTab::InstallationTab(QObject* parent) : QObject(parent), m_Engine(nullptr)
{
}

// TODO: remove this
void InstallationTab::setEngine(QQmlApplicationEngine* engine)
{
    m_Engine = engine;
}

QString InstallationTab::ds1InstallPath()
{
    return m_ds1InstallPath;
}

QString InstallationTab::ds2InstallPath()
{
    return m_ds2InstallPath;
}

void InstallationTab::setDS1InstallPath(const QString& path)
{
    if (m_ds1InstallPath == path)
        return;

    m_ds1InstallPath = path;
    emit ds1InstallPathChanged();
}

void InstallationTab::setDS2InstallPath(const QString& path)
{
    if (m_ds2InstallPath == path)
        return;

    m_ds2InstallPath = path;
    emit ds2InstallPathChanged();
}

void InstallationTab::reloadPaths()
{
    for (const char* key : registryKeyVec)
    {
        // http://stackoverflow.com/questions/34065/how-to-read-a-value-from-the-windows-registry
        if (HKEY hKey; RegOpenKeyEx(HKEY_LOCAL_MACHINE, key, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
        {
            CHAR szBuffer[MAX_PATH];
            DWORD dwBufferSize = sizeof(szBuffer);

            if (RegQueryValueEx(hKey, "EXE Path", 0, NULL, (LPBYTE)szBuffer, &dwBufferSize) == ERROR_SUCCESS)
            {
                setDS1InstallPath(szBuffer);

                QObject* qmlDS1Path = m_Engine->rootObjects().first()->findChild<QObject*>("textFieldDS1InstallPath", Qt::FindChildrenRecursively);
                if (qmlDS1Path)
                {
                    qmlDS1Path->setProperty("text", m_ds1InstallPath);
                }

                break;
            }
        }
    }

    std::cout << "DS1 Path: " << m_ds1InstallPath.toStdString() << std::endl;
    std::cout << "DS2 Path: " << m_ds2InstallPath.toStdString() << std::endl;
}

void InstallationTab::toggleOverride()
{
    if (m_overridePaths)
    {
        m_overridePaths = false;
        m_Engine->rootObjects().first()->findChild<QObject*>("textFieldDS1InstallPath", Qt::FindChildrenRecursively)->setProperty("readOnly", "true");
        m_Engine->rootObjects().first()->findChild<QObject*>("textFieldDS2InstallPath", Qt::FindChildrenRecursively)->setProperty("readOnly", "true");
    }
    else
    {
        m_overridePaths = true;
        m_Engine->rootObjects().first()->findChild<QObject*>("textFieldDS1InstallPath", Qt::FindChildrenRecursively)->setProperty("readOnly", "false");
        m_Engine->rootObjects().first()->findChild<QObject*>("textFieldDS2InstallPath", Qt::FindChildrenRecursively)->setProperty("readOnly", "false");
    }
}