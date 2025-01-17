#include "mainwindow.h"
#include "./ui_mainwindow.h"

FileManager* g_fileManager = new FileManager();

const QString VERSION_NUMBER = "0.2.0";
QList<Player*> g_playersList;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , roleAssigner(new RoleAssigner(this))
    , clipboardManager(new ClipboardManager(this))
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

    // Connect the UI buttons to the mainwindow private slots
    connect(ui->action_newfile, &QAction::triggered, this, &MainWindow::createNewCSVFile);
    connect(ui->action_openfile, &QAction::triggered, this, &MainWindow::openCSVFile);
    connect(ui->assignRolesButton, &QPushButton::clicked, this, &MainWindow::assignRoles);
    connect(ui->action_about, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(ui->action_savefile, &QAction::triggered, this, &MainWindow::saveCSVFile);
    connect(ui->copyOutputToClipboardButton, &QPushButton::clicked, this, &MainWindow::copyOutputToClipboard);

    // Connect the assignRoles signal to the RoleAssigner's assignRoles slot
    connect(this, &MainWindow::assignRolesSignal, roleAssigner, &RoleAssigner::assignRoles);

    // Connect the copyOutputToClipboard signal to the ClipboardManager's slot
    connect(this, &MainWindow::copyOutputToClipboardSignal, clipboardManager, &ClipboardManager::copyOutputToClipboard);
    connect(clipboardManager, &ClipboardManager::noResultsToCopy, this, &MainWindow::showNoResultsToCopyNotification);

    qDebug() << "<-- MainWindow::MainWindow";
}

MainWindow::~MainWindow()
{
    delete ui;
    // roleAssigner will be deleted automatically as it is a child of MainWindow
}

void MainWindow::assignRoles() {
    qDebug() << "--> MainWindow::assignRoles";
    emit assignRolesSignal(ui, g_playersList);
    qDebug() << "<-- MainWindow::assignRoles";
}

void MainWindow::copyOutputToClipboard()
{
    qDebug() << "--> MainWindow::copyOutputToClipboard";
    emit copyOutputToClipboardSignal(g_playersList);
    qDebug() << "<-- MainWindow::copyOutputToClipboard";
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

void MainWindow::showNoResultsToCopyNotification() {
    qDebug() << "--> MainWindow::showNoResultsToCopyNotification";
    QLabel *notification = new QLabel("No results to copy", this);
    notification->setStyleSheet("QLabel { background-color : lightgray; color : black; padding: 5px; }");
    notification->setAlignment(Qt::AlignCenter);
    notification->setGeometry((width() - 200) / 2, (height() - 50) / 2, 200, 50);
    notification->show();
    qDebug() << "<-- MainWindow::showNoResultsToCopyNotification";
    QTimer::singleShot(2000, notification, &QWidget::deleteLater);
}
