#include "Config.h"

bool Config::loadConfig(const QString& filePath) {
    qDebug() << "--> [Config::loadConfig]";

    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not open config file:" << filePath;
        qDebug() << "<-- [Config::loadConfig]";
        return false;
    }

    QByteArray configData = configFile.readAll();
    QJsonDocument configDoc(QJsonDocument::fromJson(configData));
    QJsonObject configObj = configDoc.object();

    versionNumber = configObj.value("versionNumber").toString("0.0.0");
    useLogFile = configObj.value("useLogFile").toBool(false);

    qDebug() << "<-- [Config::loadConfig]";
    return true;
}

QString Config::getVersionNumber() const {
    return versionNumber;
}

bool Config::getUseLogFile() const {
    return useLogFile;
}
