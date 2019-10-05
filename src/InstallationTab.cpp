
#include "InstallationTab.hpp"
#include <QQmlApplicationEngine>
#include <QNetworkRequest>
#include <QTextStream>
#include <QUrlQuery>
#include <QProcess>
#include <QDir>

#include <iostream>

// TODO: Decide whether to include Linux support?
#include <Windows.h>
#include <shlobj.h>

// Registry keys
constexpr const char* ds1RegistryKey = "Software\\Wow6432Node\\Microsoft\\Microsoft Games\\DungeonSiege\\1.0";
constexpr const char* ds1LOARegistryKey = "Software\\Wow6432Node\\Microsoft\\Microsoft Games\\Dungeon Siege Legends of Aranna\\1.0";
constexpr const char* ds2RegistryKey = "Software\\Wow6432Node\\Microsoft\\Microsoft Games\\DungeonSiege2";

InstallationTab::InstallationTab(QObject* parent) : QObject(parent), m_Engine(nullptr), m_Download(nullptr)
{
}

InstallationTab::~InstallationTab()
{
    if (m_Out.isOpen())
    {
        m_Out.cancelWriting();
        m_Out.commit();
    }
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
}

void InstallationTab::setDS2InstallPath(const QString& path)
{
    if (m_ds2InstallPath == path)
        return;

    m_ds2InstallPath = path;
}

void InstallationTab::reloadPaths()
{
    // http://stackoverflow.com/questions/34065/how-to-read-a-value-from-the-windows-registry
    if (HKEY hKey; RegOpenKeyEx(HKEY_LOCAL_MACHINE, ds1RegistryKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        CHAR szBuffer[MAX_PATH];
        DWORD dwBufferSize = sizeof(szBuffer);

        if (RegQueryValueEx(hKey, "EXE Path", 0, NULL, (LPBYTE)szBuffer, &dwBufferSize) == ERROR_SUCCESS)
        {
            QObject* qmlDS1Path = m_Engine->rootObjects().first()->findChild<QObject*>("textFieldDS1InstallPath", Qt::FindChildrenRecursively);
            if (qmlDS1Path)
            {
                setDS1InstallPath(szBuffer);
                qmlDS1Path->setProperty("text", m_ds1InstallPath);
            }
        }
    }

    // http://stackoverflow.com/questions/34065/how-to-read-a-value-from-the-windows-registry
    if (HKEY hKey; RegOpenKeyEx(HKEY_LOCAL_MACHINE, ds2RegistryKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        CHAR szBuffer[MAX_PATH];
        DWORD dwBufferSize = sizeof(szBuffer);

        if (RegQueryValueEx(hKey, "AppPath", 0, NULL, (LPBYTE)szBuffer, &dwBufferSize) == ERROR_SUCCESS)
        {
            QObject* qmlDS1Path = m_Engine->rootObjects().first()->findChild<QObject*>("textFieldDS2InstallPath", Qt::FindChildrenRecursively);
            if (qmlDS1Path)
            {
                setDS2InstallPath(szBuffer);
                qmlDS1Path->setProperty("text", m_ds2InstallPath);
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

void InstallationTab::downloadDS1TK()
{
    std::cout << "Downloading DS1TK..." << std::endl;

    downloadFile(QUrl("https://github.com/OpenSiege/SiegeHome/releases/download/0.0.1/DungeonSiegeToolkit1.7.exe"), QUrl("DungeonSiegeTookit1.7.exe"));
}

void InstallationTab::downloadDS2TK()
{
    std::cout << "Downloading DS2TK..." << std::endl;

    downloadFile(QUrl("https://github.com/OpenSiege/SiegeHome/releases/download/0.0.1/DS2TK-1.1.exe"), QUrl("DS2TK-1.1.exe"));
}

void InstallationTab::downloadFinished()
{
    // keep track of the fileName we are downloading
    static QString fileName;

    if (m_Download->error())
    {
        std::cout << "Failed to download file: " << qPrintable(m_Download->errorString());

        m_Out.cancelWriting();
    }
    else
    {
        if (isHttpRedirect())
        {
            std::cout << "Redirect happening..." << std::endl;

            // reportRedirect();

            QVariant redirectonTarget = m_Download->attribute(QNetworkRequest::RedirectionTargetAttribute);

            if (!redirectonTarget.isNull())
            {
                m_Url = m_Url.resolved(redirectonTarget.toUrl());

                // TODO: is there a better way to grab the fileName from this query?
                QUrlQuery urlQuery(m_Url);
                QString queryValue = urlQuery.queryItemValue("response-content-disposition", QUrl::FullyDecoded);
                fileName = queryValue.mid(queryValue.indexOf('=') + 1, queryValue.length());

                downloadFile(m_Url, QUrl(fileName));

                return;
            }
            else
            {
                m_Out.cancelWriting();
                m_Out.commit(); 

                return;
            }
        }
    }

    disconnect(m_Download, SIGNAL(finished()), this, SLOT(downloadReadyRead()));
    disconnect(m_Download, SIGNAL(readyRead()), this, SLOT(downloadFinished()));

    m_Download->deleteLater();
    m_Download = nullptr;

    m_Out.commit();

    std::cout << "Download finished, attempting to launch... " << qPrintable(fileName) << std::endl;

    if (!QProcess::startDetached(QDir::currentPath() + QDir::separator() + fileName))
    {
        std::cout << "Unable to start process, are we in the right dir? " << qPrintable(QDir::currentPath() + QDir::separator() + fileName) << std::endl;
    }
}

void InstallationTab::downloadReadyRead()
{
    if (m_Out.isOpen())
        m_Out.write(m_Download->readAll());
}

bool InstallationTab::isHttpRedirect() const
{
    int statusCode = m_Download->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
        || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void InstallationTab::reportRedirect()
{
    int statusCode = m_Download->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QUrl requestUrl = m_Download->request().url();
    QTextStream(stderr) << "Request: " << requestUrl.toDisplayString()
        << " was redirected with code: " << statusCode
        << '\n';

    QVariant target = m_Download->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (!target.isValid())
        return;
    QUrl redirectUrl = target.toUrl();
    if (redirectUrl.isRelative())
        redirectUrl = requestUrl.resolved(redirectUrl);
    QTextStream(stderr) << "Redirected to: " << redirectUrl.toDisplayString()
        << '\n';
}

void InstallationTab::downloadFile(QUrl remoteLocation, QUrl localFileName)
{
    if (remoteLocation.isEmpty() || localFileName.isEmpty())
        return;

    m_Url = remoteLocation;

    if (m_Out.isOpen())
    {
        m_Out.cancelWriting();
        m_Out.commit();

        disconnect(m_Download, SIGNAL(finished()), this, SLOT(downloadReadyRead()));
        disconnect(m_Download, SIGNAL(readyRead()), this, SLOT(downloadFinished()));
    }

    m_Out.setFileName(localFileName.toDisplayString(QUrl::PreferLocalFile));
    if (!m_Out.open(QIODevice::WriteOnly))
    {
        std::cerr << "Failed to write file. Permission or path problem?";

        return;
    }

    QNetworkRequest request(m_Url);

    m_Download = m_Manager.get(request);

    connect(m_Download, SIGNAL(finished()), SLOT(downloadFinished()));
    connect(m_Download, SIGNAL(readyRead()), SLOT(downloadReadyRead()));
}