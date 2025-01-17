#include "FileManager.h"

FileManager::FileManager()
    : file(nullptr)
{
}

FileManager::~FileManager() {
    delete file;
}

bool createNewCSVFile(QString& fileName) {
    return true;
}

bool saveDataToCSV(QStringList& data) {
    return true;
}

QStringList FileManager::parseCurrentFile()
{
    qDebug() << "--> FileManager::parseCurrentFile";

    QStringList fileContents;

    if (!file || !file->isOpen() || !(file->openMode() & QIODevice::ReadOnly)) {
        qDebug() << "   File is not open for reading.";
        return fileContents;
    }

    // Reset the file's position to the beginning before reading
    file->seek(0);

    QTextStream in(file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << "   " << line;
        fileContents.append(line);
    }

    qDebug() << "    " << fileContents;
    return fileContents;

    qDebug() << "<-- FileManager::parseCurrentFile";
}

QList<QStringList> FileManager::parseCurrentFileIntoColumns()
{
    qDebug() << "--> FileManager::parseCurrentFileIntoColumns";
    file->seek(0);
    QTextStream in(file);
    bool isFirstLine = true;
    QList<QStringList> columns;

    // Read the file line by line
    while (!in.atEnd()) {
        QString line = in.readLine();

        // Split each line into cells using comma as delimiter
        QStringList cells = line.split(",");
        qDebug() << "    Line cells:" << cells;

        // Initialize columns based on the number of cells in the first row
        if (isFirstLine) {
            for (int i = 0; i < cells.size(); ++i) {
                columns.append(QStringList());  // Create a new list for each column
            }
            isFirstLine = false;
        }

        // Add each cell to the appropriate column
        for (int i = 0; i < cells.size(); ++i) {
            columns[i].append(cells[i]);
        }
    }

    qDebug() << "    Columns:" << columns;
    qDebug() << "     File data parsed into columns.";
    qDebug() << "<-- FileManager::parseCurrentFileIntoColumns";
    return columns;
}

QList<Player*> FileManager::parseCurrentFileIntoPlayers() {
    qDebug() << "--> FileManager::parseCurrentFileIntoPlayers";

    QList<Player*> players;

    if (!file || !file->isOpen())
        return players;

    file->seek(0);
    QTextStream in(file);
    QString line;

    // Skip the first line (headers)
    in.readLine();

    while (!in.atEnd()) {
        line = in.readLine();
        QStringList fields = line.split(',');

        // Ensure the line has the correct number of fields
        if (fields.size() == 5) {
            QString name = fields[0].trimmed();
            int totalGames = fields[1].trimmed().toInt();
            int vanguardCount = fields[2].trimmed().toInt();
            int duelistCount = fields[3].trimmed().toInt();
            int strategistCount = fields[4].trimmed().toInt();

            // Create a Player object and add to the list
            Player* player = new Player(name);
            player->setTotalGames(totalGames);
            player->setVanguardCount(vanguardCount);
            player->setDuelistCount(duelistCount);
            player->setStrategistCount(strategistCount);

            players.append(player);
        } else {
            qDebug() << "   Skipping invalid line: " << line;
        }
    }

    qDebug() << "<-- FileManager::parseCurrentFileIntoPlayers";
    return players;
}

bool FileManager::savePlayersToCSV(const QList<Player>& players, const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << "Player Name:,Total Games:,Vanguard Count:,Duelist Count:,Strategist Count:\n";
    for (const Player& player : players) {
        out << player.toCSVString() << "\n";
    }

    file.close();
    return true;
}

QFile* FileManager::getFile()
{
    return file;
}

void FileManager::setFile(QFile* newFile)
{
    qDebug() << "--> FileManager::setFile";

    qDebug() << "    Setting new file object.";
    this->file = newFile;
    if (file) {
        filePath = file->fileName();
        qDebug() << "    New filepath: " << filePath;
    }

    qDebug() << "<-- FileManager::setFile";
}
