
#ifndef INSTALLATION_TAB_HPP
#define INSTALLATION_TAB_HPP

#include <QObject>
#include <QString>


class QQmlApplicationEngine;
class InstallationTab : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ds1InstallPath READ ds1InstallPath WRITE setDS1InstallPath NOTIFY ds1InstallPathChanged)
    Q_PROPERTY(QString ds2InstallPath READ ds2InstallPath WRITE setDS2InstallPath NOTIFY ds2InstallPathChanged)

public:
    explicit InstallationTab(QObject* parent = nullptr);

    QString ds1InstallPath();
    QString ds2InstallPath();

    void setDS1InstallPath(const QString& path);
    void setDS2InstallPath(const QString& path);

    Q_INVOKABLE void reloadPaths();
    Q_INVOKABLE void toggleOverride();

    // TODO: remove this
    // https://developer.blackberry.com/native/documentation/dev/integrating_cpp_qml/
    // Looks like we should be encapsulating the actual qml load into an application type class to map our QML types
    void setEngine(QQmlApplicationEngine* engine);

signals:
    void ds1InstallPathChanged();
    void ds2InstallPathChanged();

private:

    QQmlApplicationEngine* m_Engine;

    QString m_ds1InstallPath;
    QString m_ds2InstallPath;

    bool m_overridePaths = false;
};

#endif // INSTALLATION_TAB_HPP