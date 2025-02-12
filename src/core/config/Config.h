#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>

class Config
{
public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    bool loadConfig(const QString& filePath);
    QString getVersionNumber() const;
    bool getUseLogFile() const;

private:
    Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    QString versionNumber;
    bool useLogFile;
};

#endif // CONFIG_H
