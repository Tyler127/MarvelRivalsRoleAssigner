#include "RoleAssigner.h"

RoleAssigner::RoleAssigner(QObject* parent) : QObject(parent) {
    // Constructor implementation (if any)
}

void RoleAssigner::assignRoles(Ui::MainWindow* ui, QList<Player*>& playersList) {
    qDebug() << "--> RoleAssigner::assignRoles";

    // Reset each player's assigned role to "None"
    for (Player* player : playersList) {
        player->setAssignedRole("None");
    }
    qDebug() << "    Reset all player roles to 'None'.";

    // Create a list of the selected strings of player names from the comboboxes
    QSet<QString> selectedStringsSet;
    selectedStringsSet.insert(ui->playerBox_1->currentText().trimmed());
    selectedStringsSet.insert(ui->playerBox_2->currentText().trimmed());
    selectedStringsSet.insert(ui->playerBox_3->currentText().trimmed());
    selectedStringsSet.insert(ui->playerBox_4->currentText().trimmed());
    selectedStringsSet.insert(ui->playerBox_5->currentText().trimmed());
    selectedStringsSet.insert(ui->playerBox_6->currentText().trimmed());

    // Remove empty strings from the set of selected names
    selectedStringsSet.remove("");
    qDebug() << "    Selected names: " << selectedStringsSet;

    // Create a list of the selected players by finding them based on name
    QList<Player*> selectedPlayersList;
    for (Player* player : playersList) {
        if (selectedStringsSet.contains(player->getName())) {
            selectedPlayersList.append(player);
            qDebug() << "    " << player->getName() << " was selected in the UI";

            // Remove so that only names never used previously remain in the set
            selectedStringsSet.remove(player->getName());
        }
    }
    qDebug() << "    Selected names that aren't player objects: " << selectedStringsSet;

    // Remove any blank strings and add new player for never before seen names
    qDebug() << "    Before creating new players selectedPlayersList:";
    for (Player* player : selectedPlayersList) { qDebug() << "       " << player->getName(); }
    for (QString string : selectedStringsSet) {
        Player* newPlayer = new Player(string);
        playersList.append(newPlayer);
        selectedPlayersList.append(newPlayer);
        qDebug() << "    Created new player: " << newPlayer->toString();
    }
    qDebug() << "    After creating new players selectedPlayersList:";
    for (Player* player : selectedPlayersList) { qDebug() << "       " << player->getName(); }

    // Setup the amount of each role to be selected
    // TODO: allow for configuration of role counts in the UI
    int vanguardCount = 2;
    int duelistCount = 2;
    int strategistCount = 2;

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

    // Shuffle list of selected players for more randomness
    shufflePlayers(selectedPlayersList);

    // Assign the first half of players to their least played role
    assignHandpickedRoles(selectedPlayersList, numPlayersToHandpick, vanguardAssignments, duelistAssignments, strategistAssignments);

    // Assign the next half of players to random roles
    assignRandomRoles(selectedPlayersList, numPlayersToHandpick, vanguardAssignments, duelistAssignments, strategistAssignments);

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

        qDebug() << "       Player at end: " << player->toString();
    }

    qDebug() << "<-- RoleAssigner::assignRoles";
}

void RoleAssigner::shufflePlayers(QList<Player*>& playersList) {
    qDebug() << "--> RoleAssigner::shufflePlayers";

    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::shuffle(playersList.begin(), playersList.end(), randomGenerator);

    qDebug() << "<-- RoleAssigner::shufflePlayers";
}

void RoleAssigner::assignHandpickedRoles(QList<Player*>& playersList, int numPlayersToHandpick, int& vanguardAssignments, int& duelistAssignments, int& strategistAssignments) {
    qDebug() << "--> RoleAssigner::assignHandpickedRoles";

    for (int i = 0; i < numPlayersToHandpick; i++) {
        Player* currentPlayer = playersList[i];
        qDebug() << "    Assigning handpick: " << currentPlayer->getName();

        int minCount = std::min({currentPlayer->getVanguardCount(), currentPlayer->getDuelistCount(), currentPlayer->getStrategistCount()});
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

        bool roleSelectionValid = false;
        int randomNum;
        int attemptsMade = 0;
        std::random_device randomDevice;
        std::mt19937 randomGenerator(randomDevice());
        while (!roleSelectionValid && attemptsMade < 100) {
            std::uniform_int_distribution<> intDist(0, possibleRoles.size() - 1);
            randomNum = possibleRoles[intDist(randomGenerator)];
            attemptsMade++;
            qDebug() << "       Selected Role: " << randomNum;

            if ((randomNum == 0 && vanguardAssignments + 1 <= 2) ||
                (randomNum == 1 && duelistAssignments + 1 <= 2) ||
                (randomNum == 2 && strategistAssignments + 1 <= 2) ||
                attemptsMade > 100) {
                roleSelectionValid = true;

                if (attemptsMade > 90) { qDebug() << "ATTEMPTS MADE MAXXED"; };
                qDebug() << "       Player counts bfr: " << currentPlayer->toString();

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
                qDebug() << "       Player counts aft: " << currentPlayer->toString();
            }
        }
    }

    qDebug() << "<-- RoleAssigner::assignHandpickedRoles";
}

void RoleAssigner::assignRandomRoles(QList<Player*>& playersList, int numPlayersToHandpick, int& vanguardAssignments, int& duelistAssignments, int& strategistAssignments) {
    qDebug() << "--> RoleAssigner::assignRandomRoles";
    
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_int_distribution<> intDist(0, 2);

    for (int i = numPlayersToHandpick; i < playersList.size(); i++) {
        Player* currentPlayer = playersList[i];
        qDebug() << "    Assigning randomly: " << currentPlayer->getName();

        bool roleSelectionValid = false;
        int randomNum;
        while (!roleSelectionValid) {
            randomNum = intDist(randomGenerator);
            qDebug() << "       Selected Role: " << randomNum;

            if ((randomNum == 0 && vanguardAssignments + 1 <= 2) ||
                (randomNum == 1 && duelistAssignments + 1 <= 2) ||
                (randomNum == 2 && strategistAssignments + 1 <= 2)) {
                roleSelectionValid = true;
                qDebug() << "       Player counts bfr: " << currentPlayer->toString();

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
                qDebug() << "       Player counts aft: " << currentPlayer->toString();
            }
        }
    }

    qDebug() << "<-- RoleAssigner::assignRandomRoles";
}
