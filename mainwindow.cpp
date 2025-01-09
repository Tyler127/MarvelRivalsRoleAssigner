#include "mainwindow.h"
#include "./ui_mainwindow.h"

filemanager* g_fileManager = new filemanager();
QList<Player> g_playersList;

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

    //connect(ui->action_savefile, &QAction::triggered, this, &MainWindow::openCSVFile);

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
        "<a href=\"https://github.com/Tyler127/MarvelRivalsRoleAssigner\">Marvel Rivals Role Assigner Github Repo</a><br>"
        "Version: 1.0.0<br>"
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

    if (fileName.isEmpty()) {
        return;
    }

    // Check if the file exists already and ask to overwrite it
    if (QFile::exists(fileName)) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Overwrite File", "File already exists. Do you want to overwrite it?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }

    // Create and open the file for reading and writing
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Unable to create the file.");
        return;
    }

    // Inform the user that the file was created
    QMessageBox::information(this, "File Created", "New CSV file created successfully.");

    processCSVFile(&file, fileName);

    qDebug() << "<-- MainWindow::createNewCSVFile";
}


void MainWindow::openCSVFile()
{
    qDebug() << "--> MainWindow::openCSVFile";

    // Get file location from user
    QString fileName = QFileDialog::getOpenFileName(this, "Open CSV File", "", "CSV Files (*.csv);;All Files (*)");

    if (fileName.isEmpty()) {
        return;
    }

    // Open the file for reading and writing
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
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

    // Set the display text to the file's data
    QStringList fileDisplayData = g_fileManager->parseCurrentFile();
    QString displayString = fileDisplayData.join("\n");
    ui->fileContentsLabel->setText(displayString);
    qDebug() << "   Display String: " << displayString;
    qDebug() << "   File content display set.";

    // Setup the UI with the file's data
    QList<QStringList> fileData = g_fileManager->parseCurrentFileIntoColumns();

    setupUIFromCSV(fileData);

    // Create a list of players
    g_playersList = g_fileManager->parseCurrentFileIntoPlayers();
    for (Player player : g_playersList) {
        qDebug() << "   " << player;
    }

    qDebug() << "<-- MainWindow::processCSVFile";
}


void MainWindow::setupUIFromCSV(QList<QStringList> fileData)
{
    qDebug() << "--> MainWindow::setupUIFromCSV";

    // Exit the function if no file data was parsed
    if (!fileData.size()) { return; }

    QStringList playerNames = fileData[0];
    playerNames.removeFirst(); // Remove the column header from the list

    qDebug() << "   " << playerNames;

    // Set the initial text for the comboboxes
    // TODO: remove all previous combobox options if there are any before adding any new ones. This is for when a new file is opened.
    QString prevName;
    for (int i=0; i < 6 && i < playerNames.size(); ++i) {
        QString name = playerNames[i];

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

    // Add each name as a dropdown option to the comboboxes
    for (QString name : playerNames) {
        ui->playerBox_1->addItem(name);
        ui->playerBox_2->addItem(name);
        ui->playerBox_3->addItem(name);
        ui->playerBox_4->addItem(name);
        ui->playerBox_5->addItem(name);
        ui->playerBox_6->addItem(name);
    }

    qDebug() << "<-- MainWindow::setupUIFromCSV";
}


void MainWindow::assignRoles() {
    qDebug() << "--> MainWindow::assignRoles";

    // Create a list of the selected strings of player names from the comboboxes
    QSet<QString> selectedStringsSet;
    selectedStringsSet.insert(ui->playerBox_1->currentText().trimmed());
    selectedStringsSet.insert(ui->playerBox_2->currentText().trimmed());
    selectedStringsSet.insert(ui->playerBox_3->currentText().trimmed());
    selectedStringsSet.insert(ui->playerBox_4->currentText().trimmed());
    selectedStringsSet.insert(ui->playerBox_5->currentText().trimmed());
    selectedStringsSet.insert(ui->playerBox_6->currentText().trimmed());
    qDebug() << "    Selected names: " << selectedStringsSet;

    // Create a list of the selected players by finding them based on name
    QList<Player*> selectedPlayersList;
    for (Player& player : g_playersList) {
        if (selectedStringsSet.contains(player.getName())) {
            selectedPlayersList.append(&player);
            qDebug() << "   " << player.getName() << " was selected in the UI";

            // Remove so that only names never used previously remain in the set
            selectedStringsSet.remove(player.getName());
        }
    }
    qDebug() << "    Selected names that aren't player objects: " << selectedStringsSet;

    // Remove any blank strings and add new player for never before seen names
    for (QString string : selectedStringsSet) {
        if (string != "") {
            Player newPlayer(string);
            g_playersList.append(newPlayer);
            selectedPlayersList.append(&newPlayer);
            qDebug() << "    Created new player" << newPlayer;
        }
    }

    // Setup the amount of each role to be selected
    // TODO: allow for configuration of role counts in the UI
    int vanguardCount = 2;
    int duelistCount = 2;
    int strategistCount = 2;

    // Setup random number generation
    std::random_device randomDevice; // Creates seed for the generator using hardware entropy
    std::mt19937 randomGenerator(randomDevice()); // A random number generator
    std::uniform_int_distribution<> intDist(0, 2); // 0=vanguard, 1=duelist, 2=strategist

    // Shuffle list of selected players for more randomness
    std::shuffle(selectedPlayersList.begin(), selectedPlayersList.end(), randomGenerator);
    qDebug() << "    Shuffled players list:";
    for (Player* player : selectedPlayersList) { qDebug() << "       " << player->getName(); }

    // Track the amount of times each role has been assigned
    int vanguardAssignments = 0;
    int duelistAssignments = 0;
    int strategistAssignments = 0;

    // Calculate the amount of players that need to be handpicked and randomized
    int numPlayersToHandpick = std::floor(static_cast<double>(selectedPlayersList.size()) / 2);
    int numPlayersToRandomize = std::ceil(static_cast<double>(selectedPlayersList.size()) / 2);
    qDebug() << "    selectedPlayerList.size(): " << selectedPlayersList.size();
    qDebug() << "    Num players to handpick: " << numPlayersToHandpick;
    qDebug() << "    Num players to randomize: " << numPlayersToRandomize;

    // Assign the first half of players to their least played role
    for (int i=0; i < numPlayersToHandpick; i++) {
        Player* currentPlayer = selectedPlayersList[i];
        qDebug() << "     Assigning handpick: " << currentPlayer->getName();

        // Find which role/s the player has been selected for the least
        int minCount = std::min({currentPlayer->getVanguardCount(), currentPlayer->getDuelistCount(), currentPlayer->getStrategistCount()});

        // Add to a list all roles that have the least amount of occurances to account for the case of a tie
        std::vector<int> possibleRoles;
        if (currentPlayer->getVanguardCount() == minCount) {
            possibleRoles.push_back(0);
        }
        if (currentPlayer->getDuelistCount() == minCount) {
            possibleRoles.push_back(1);
        }
        if (currentPlayer->getStrategistCount() == minCount) {
            possibleRoles.push_back(2);
        }

        // Loop until a valid role selection is made. Or defaults to whatever works if a limit of attempts is performed.
        // TODO: fix bug that occurs when two players get assigned to the same role and then that role is the only mincount role for the third player
        //         this bug then would loop endlessly. currently have the attemptsMade code in below as a hotfix but it results in 3 players with the
        //         same role as it just uses whichever role that player was assigned on the previous execution of this function.
        //         example: sam and yohann both are duelists from this below loop. jacob then has played duelist the least and is the third player to be
        //                  selected for. therefore cannot force jacob into duelist role and results in him being what he was last game with the hotfix.
        bool roleSelectionValid = false;
        int randomNum;
        int attemptsMade = 0;
        while (!roleSelectionValid && attemptsMade < 100) {
            std::uniform_int_distribution<> intDist(0, possibleRoles.size() - 1);
            randomNum = possibleRoles[intDist(randomGenerator)];
            attemptsMade++;
            qDebug() << "       Selected Role: " << randomNum;

            // The role selection is valid if the previous amount of assignments to
            // a role plus one is less than or equal to the maximum for that role
            if ((randomNum == 0 && vanguardAssignments + 1 <= vanguardCount) ||
                (randomNum == 1 && duelistAssignments + 1 <= duelistCount) ||
                (randomNum == 2 && strategistAssignments + 1 <= strategistCount) ||
                attemptsMade > 100) {
                roleSelectionValid = true;

                if (attemptsMade > 90) { qDebug() << "ATTEMPTS MADE MAXXED"; };

                // Update the role assignment totals and the player's role based on the selected role
                if (randomNum == 0) {
                    vanguardAssignments += 1;
                    currentPlayer->setAssignedRole("Vanguard");
                    currentPlayer->setVanguardCount(currentPlayer->getVanguardCount() + 1);
                }
                if (randomNum == 1) {
                    duelistAssignments += 1;
                    currentPlayer->setAssignedRole("Duelist");
                    currentPlayer->setDuelistCount(currentPlayer->getDuelistCount() + 1);
                }
                if (randomNum == 2) {
                    strategistAssignments += 1;
                    currentPlayer->setAssignedRole("Strategist");
                    currentPlayer->setStrategistCount(currentPlayer->getStrategistCount() + 1);
                }

                currentPlayer->setTotalGames(currentPlayer->getTotalGames() + 1);
            }
        }
    }

    // Assign the next half of players to random roles
    for (int i=numPlayersToHandpick; i < selectedPlayersList.size(); i++) {
        Player* currentPlayer = selectedPlayersList[i];
        qDebug() << "     Assigning randomly: " << currentPlayer->getName();

        // Loop until a valid role selection is made
        bool roleSelectionValid = false;
        int randomNum;
        while (!roleSelectionValid) {
            randomNum = intDist(randomGenerator);
            qDebug() << "       Selected Role: " << randomNum;

            // The role selection is valid if the previous amount of assignments to
            // a role plus one is less than or equal to the maximum for that role
            if ((randomNum == 0 && vanguardAssignments + 1 <= vanguardCount) ||
                (randomNum == 1 && duelistAssignments + 1 <= duelistCount) ||
                (randomNum == 2 && strategistAssignments + 1 <= strategistCount)) {
                roleSelectionValid = true;

                // Update the role assignment totals and the player's role based on the selected role
                if (randomNum == 0) {
                    vanguardAssignments += 1;
                    currentPlayer->setAssignedRole("Vanguard");
                }
                if (randomNum == 1) {
                    duelistAssignments += 1;
                    currentPlayer->setAssignedRole("Duelist");
                }
                if (randomNum == 2) {
                    strategistAssignments += 1;
                    currentPlayer->setAssignedRole("Strategist");
                }
            }
        }
    }

    // Update the role labels
    for (Player* player : selectedPlayersList) {
        if (ui->playerBox_1->currentText().trimmed() == player->getName()) {
            ui->playerRole_1->setText(player->getAssignedRole());
        } else if (ui->playerBox_2->currentText().trimmed() == player->getName()) {
            ui->playerRole_2->setText(player->getAssignedRole());
        } else if (ui->playerBox_3->currentText().trimmed() == player->getName()) {
            ui->playerRole_3->setText(player->getAssignedRole());
        } else if (ui->playerBox_4->currentText().trimmed() == player->getName()) {
            ui->playerRole_4->setText(player->getAssignedRole());
        } else if (ui->playerBox_5->currentText().trimmed() == player->getName()) {
            ui->playerRole_5->setText(player->getAssignedRole());
        } else if (ui->playerBox_6->currentText().trimmed() == player->getName()) {
            ui->playerRole_6->setText(player->getAssignedRole());
        }
    }

    qDebug() << "<-- MainWindow::assignRoles";
}
