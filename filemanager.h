#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QDir>
#include <QRegularExpression>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include "Player.h"

class filemanager
{
public:
    filemanager();
    ~filemanager();

    bool createNewCSVFile(const QString &fileName);
    bool saveDataToCSV(const QStringList &data);
    QStringList parseCurrentFile();
    QList<QStringList> parseCurrentFileIntoColumns();
    QList<Player> parseCurrentFileIntoPlayers();
    QFile* getFile();
    void setFile(QFile* newFile);
private:
    QFile *file;
    QString filePath;
};

#endif // FILEMANAGER_H
