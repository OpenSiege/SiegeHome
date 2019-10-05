
#ifndef INSTALLATION_TAB_HPP
#define INSTALLATION_TAB_HPP

#include <QObject>
#include <QString>
#include <QSaveFile>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class QQmlApplicationEngine;
class InstallationTab : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ds1InstallPath READ ds1InstallPath WRITE setDS1InstallPath NOTIFY ds1InstallPathChanged)
    Q_PROPERTY(QString ds2InstallPath READ ds2InstallPath WRITE setDS2InstallPath NOTIFY ds2InstallPathChanged)

public:
    explicit InstallationTab(QObject* parent = nullptr);
    ~InstallationTab();

    QString ds1InstallPath();
    QString ds2InstallPath();

    void setDS1InstallPath(const QString& path);
    void setDS2InstallPath(const QString& path);

    Q_INVOKABLE void reloadPaths();
    Q_INVOKABLE void toggleOverride();
    Q_INVOKABLE void downloadDS1TK();

    // TODO: remove this
    // https://developer.blackberry.com/native/documentation/dev/integrating_cpp_qml/
    // Looks like we should be encapsulating the actual qml load into an application type class to map our QML types
    void setEngine(QQmlApplicationEngine* engine);

signals:
    void ds1InstallPathChanged();
    void ds2InstallPathChanged();

private slots:
    void downloadFinished();
    void downloadReadyRead();

private:

    bool isHttpRedirect() const;
    void reportRedirect();

    void downloadFile(QUrl remoteLocation, QUrl localFileName);

    QQmlApplicationEngine* m_Engine;

    QString m_ds1InstallPath;
    QString m_ds2InstallPath;

    bool m_overridePaths = false;

    QSaveFile m_Out;
    QUrl m_Url;
    QNetworkAccessManager m_Manager;
    QNetworkReply* m_Download;
};

#endif // INSTALLATION_TAB_HPP