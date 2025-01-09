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

    /**
     *  @brief Creates a new CSV file with the given filename
     *  @param fileName The name for the new file.
     *  @return True if the file was successfully created, otherwise false.
     */
    bool createNewCSVFile(QString& fileName);

    /**
     *  @brief Saves data to the current file.
     *  @param data The data to be saved to the file.
     *  @return True if the file was successfully saved, otherwise false.
     */
    bool saveDataToCSV(QStringList& data);

    /**
     *  @brief Creates a list of strings where each string contains one line of the file.
     *  @return A list of the file's lines.
     */
    QStringList parseCurrentFile();

    /**
     *  @brief Creates a list of lists where each sublist contains all of the strings
     *  in each column of the file.
     *  @return The file parsed into columns.
     */
    QList<QStringList> parseCurrentFileIntoColumns();

    /**
     *  @brief Creates a list of Player objects using the file's data.
     *  @return A list of player objects.
     */
    QList<Player> parseCurrentFileIntoPlayers();

    /**
     *  @brief Returns the current file object.
     */
    QFile* getFile();

    /**
     *  @brief Sets the current file to the given file object.
     *  @param newFile The new QFile* object.
     */
    void setFile(QFile* newFile);
private:
    QFile *file;
    QString filePath;
};

#endif // FILEMANAGER_H
