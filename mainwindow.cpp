#include "mainwindow.h"
#include "./ui_mainwindow.h"

FileManager* g_fileManager = new FileManager();
RoleAssigner* g_roleAssigner = new RoleAssigner();

const QString VERSION_NUMBER = "0.2.0";
QList<Player*> g_playersList;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "--> MainWindow::MainWindow";

    ui->setupUi(this);

    // Change the window title and icon
    this->setWindowTitle("Marvel Rivals Role Assigner");

    // Set the fixed size of the main window
    this->setFixedSize(600, 500);

    // Remove the resize handle and make the window non-resizable
    //setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    //setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    // Set box tabbing order
    ui->playerBox_1->setFocusPolicy(Qt::StrongFocus);
    ui->playerBox_2->setFocusPolicy(Qt::StrongFocus);
    ui->playerBox_3->setFocusPolicy(Qt::StrongFocus);
    ui->playerBox_4->setFocusPolicy(Qt::StrongFocus);
    ui->playerBox_5->setFocusPolicy(Qt::StrongFocus);
    ui->playerBox_6->setFocusPolicy(Qt::StrongFocus);
    QWidget::setTabOrder(ui->playerBox_1, ui->playerBox_2);
    QWidget::setTabOrder(ui->playerBox_2, ui->playerBox_3);
    QWidget::setTabOrder(ui->playerBox_3, ui->playerBox_4);
    QWidget::setTabOrder(ui->playerBox_4, ui->playerBox_5);
    QWidget::setTabOrder(ui->playerBox_5, ui->playerBox_6);

    // Connect the slots
    connect(ui->action_newfile, &QAction::triggered, this, &MainWindow::createNewCSVFile);
    connect(ui->action_openfile, &QAction::triggered, this, &MainWindow::openCSVFile);
    connect(ui->assignRolesButton, &QPushButton::clicked, this, &MainWindow::assignRoles);
    connect(ui->action_about, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(ui->action_savefile, &QAction::triggered, this, &MainWindow::saveCSVFile);
    connect(ui->copyOutputToClipboardButton, &QPushButton::clicked, this, &MainWindow::copyOutputToClipboard);

    qDebug() << "<-- MainWindow::MainWindow";
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAboutDialog() {
    qDebug() << "--> MainWindow::showAboutDialog";

    QMessageBox aboutBox;
    aboutBox.setWindowTitle("About This Application");
    aboutBox.setTextFormat(Qt::RichText);
    aboutBox.setText(
        "Marvel Rivals Role Assigner<br><br>"
        "Developed by Tyler Larson<br>"
        "<a href=\"https://github.com/Tyler127/MarvelRivalsRoleAssigner\">Marvel Rivals Role Assigner Github Repository</a><br>"
        "Version: " + VERSION_NUMBER + "<br>"
        "© 2025"
    );
    aboutBox.setStandardButtons(QMessageBox::Ok);
    aboutBox.exec();

    qDebug() << "<-- MainWindow::showAboutDialog";
}

void MainWindow::createNewCSVFile()
{
    qDebug() << "--> MainWindow::createNewCSVFile";

    // Get file location from user
    QString fileName = QFileDialog::getSaveFileName(this, "Create New CSV File", "", "CSV Files (*.csv);;All Files (*)");
    qDebug() << "    Selected file name: " << fileName;

    if (fileName.isEmpty()) {
        qDebug() << "    ERROR: No file name provided.";
        return;
    }

    // Check if the file exists already and ask to overwrite it
    if (QFile::exists(fileName)) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Overwrite File", "File already exists. Do you want to overwrite it?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            qDebug() << "    Message Box Response: User chose not to overwrite the existing file.";
            return;
        }
    }

    // Create and open the file for reading and writing
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qCritical() << "    CRITICAL ERROR: Unable to create the file.";
        QMessageBox::critical(this, "Error", "Unable to create the file.");
        return;
    }

    // Inform the user that the file was created
    QMessageBox::information(this, "File Created", "New CSV file created successfully.");
    qDebug() << "    New CSV file created successfully.";

    processCSVFile(&file, fileName);

    qDebug() << "<-- MainWindow::createNewCSVFile";
}


void MainWindow::openCSVFile()
{
    qDebug() << "--> MainWindow::openCSVFile";

    // Get file location from user
    QString fileName = QFileDialog::getOpenFileName(this, "Open CSV File", "", "CSV Files (*.csv);;All Files (*)");
    qDebug() << "    Selected file name: " << fileName;

    if (fileName.isEmpty()) {
        qDebug() << "    ERROR: No file name provided.";
        return;
    }

    // Open the file for reading and writing
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qCritical() << "    CRITICAL ERROR: Unable to open the file.";
        QMessageBox::critical(this, "Error", "Unable to open the file.");
        return;
    }

    processCSVFile(&file, fileName);

    qDebug() << "<-- MainWindow::openCSVFile";
}


void MainWindow::processCSVFile(QFile* file, QString fileName)
{
    qDebug() << "--> MainWindow::processCSVFile";

    // Set up the file and label
    g_fileManager->setFile(file);
    QFileInfo fileInfo(fileName);
    QString baseFileName = fileInfo.fileName();
    ui->currentFileNameLabel->setText(baseFileName);
    qDebug() << "    Current file name label set to: " << baseFileName;

    // Set the display text to the file's data
    QStringList fileDisplayData = g_fileManager->parseCurrentFile();
    QString displayString = fileDisplayData.join("\n");
    ui->fileContentsLabel->setText(displayString);
    qDebug() << "    Display String: " << displayString;
    qDebug() << "    File content display set.";

    // Setup the UI with the file's data
    QList<QStringList> fileData = g_fileManager->parseCurrentFileIntoColumns();

    setupUIFromCSV(fileData);

    // Create a list of players
    g_playersList = g_fileManager->parseCurrentFileIntoPlayers();
    for (Player* player : g_playersList) {
        qDebug() << "    Player: " << player->toString();
    }

    qDebug() << "<-- MainWindow::processCSVFile";
}


void MainWindow::setupUIFromCSV(QList<QStringList> fileData)
{
    qDebug() << "--> MainWindow::setupUIFromCSV";

    // Exit the function if no file data was parsed
    if (!fileData.size()) {
        qDebug() << "    No file data parsed.";
        return;
    }

    QStringList playerNames = fileData[0];
    playerNames.removeFirst(); // Remove the column header from the list
    qDebug() << "    Player names: " << playerNames;

    // Set the initial text for the comboboxes
    // TODO: remove all previous combobox options if there are any before adding any new ones. This is for when a new file is opened.
    QString prevName;
    for (int i=0; i < 6 && i < playerNames.size(); ++i) {
        QString name = playerNames[i];
        qDebug() << "    Adding player name to combobox: " << name;

        switch(i) {
            case 0:
                ui->playerBox_1->addItem(name);
                break;
            case 1:
                ui->playerBox_2->addItem(name);
                break;
            case 2:
                ui->playerBox_3->addItem(name);
                break;
            case 3:
                ui->playerBox_4->addItem(name);
                break;
            case 4:
                ui->playerBox_5->addItem(name);
                break;
            case 5:
                ui->playerBox_6->addItem(name);
                break;
        }
    }

    // Add a blank option
    ui->playerBox_1->addItem("");
    ui->playerBox_2->addItem("");
    ui->playerBox_3->addItem("");
    ui->playerBox_4->addItem("");
    ui->playerBox_5->addItem("");
    ui->playerBox_6->addItem("");
    qDebug() << "    Added blank options to comboboxes.";

    // Add each name as a dropdown option to the comboboxes
    for (QString name : playerNames) {
        ui->playerBox_1->addItem(name);
        ui->playerBox_2->addItem(name);
        ui->playerBox_3->addItem(name);
        ui->playerBox_4->addItem(name);
        ui->playerBox_5->addItem(name);
        ui->playerBox_6->addItem(name);
    }
    qDebug() << "    Added player names to comboboxes.";

    qDebug() << "<-- MainWindow::setupUIFromCSV";
}


void MainWindow::assignRoles() {
    g_roleAssigner->assignRoles(ui, g_playersList);
}

void MainWindow::saveCSVFile()
{
    qDebug() << "--> MainWindow::saveCSVFile";

    QString fileName = QFileDialog::getSaveFileName(this, "Save CSV File", "", "CSV Files (*.csv);;All Files (*)");
    qDebug() << "    Selected file name: " << fileName;

    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Unable to save the file.");
        return;
    }

    QTextStream out(&file);
    out << "Player Name:,Total Games:,Vanguard Count:,Duelist Count:,Strategist Count:\n";
    for (Player* player : g_playersList) {
        out << player->toCSVString() << "\n";
    }

    file.close();
    QMessageBox::information(this, "File Saved", "CSV file saved successfully.");

    qDebug() << "<-- MainWindow::saveCSVFile";
}

void MainWindow::copyOutputToClipboard()
{
    qDebug() << "--> MainWindow::copyOutputToClipboard";

    // Display a notification saying that there was nothing to be copied
    if (g_playersList.isEmpty()) {
        QLabel *notification = new QLabel("No results to copy", this);
        notification->setStyleSheet("QLabel { background-color : lightgray; color : black; padding: 5px; }");
        notification->setAlignment(Qt::AlignCenter);
        notification->setGeometry((width() - 200) / 2, (height() - 50) / 2, 200, 50);
        notification->show();
        QTimer::singleShot(2000, notification, &QWidget::deleteLater);

        qDebug() << "<-- MainWindow::copyOutputToClipboard";
        return;
    }

    // Initialize the output string
    QString clipboardText = "```\n";
    clipboardText += QString("%1 | %2 | %3 | %4\n")
        .arg("Name", -10)
        .arg("Role", -10)
        .arg("% of Games", -10)
        .arg("Times Played", -10);
    clipboardText += "---------------------------------------------\n";

    // Go through each player and if they have an assigned role add their info to the output string
    for (Player* player : g_playersList) {
        qDebug() << "       Player: " << player;

        int roleCount = 0;
        if (player->getAssignedRole() == "Vanguard") {
            roleCount = player->getVanguardCount();
        } else if (player->getAssignedRole() == "Duelist") {
            roleCount = player->getDuelistCount();
        } else if (player->getAssignedRole() == "Strategist") {
            roleCount = player->getStrategistCount();
        }

        if (roleCount) {
            qDebug() << "       Player copied: " << player->getName();
            double percentagePlayed = (player->getTotalGames() > 0) ? (static_cast<double>(roleCount) / player->getTotalGames()) * 100 : 0;

            clipboardText += QString("%1 | %2 | %3 | %4\n")
                .arg(player->getName(), -10)
                .arg(player->getAssignedRole(), -10)
                .arg(QString::number(percentagePlayed, 'f', 2), -10)
                .arg(roleCount, -10);
        }
    }
    clipboardText += "```";

    // Send the text to the clipboard
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(clipboardText);

    // Show a temporary success notification
    QLabel *notification = new QLabel("Copied to Clipboard", this);
    notification->setStyleSheet("QLabel { background-color : lightgray; color : black; padding: 5px; }");
    notification->setAlignment(Qt::AlignCenter);
    notification->setGeometry((width() - 200) / 2, (height() - 50) / 2, 200, 50);
    notification->show();
    QTimer::singleShot(2000, notification, &QWidget::deleteLater);

    qDebug() << "<-- MainWindow::copyOutputToClipboard";
}
