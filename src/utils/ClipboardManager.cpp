#include "ClipboardManager.h"

ClipboardManager::ClipboardManager(QObject *parent)
    : QObject{parent}
{}

void ClipboardManager::copyOutputToClipboard(QList<Player*>& playersList)
{
    qDebug() << "--> [ClipboardManager::copyOutputToClipboard]";

    if (playersList.isEmpty()) {
        emit noResultsToCopy();
        qDebug() << "<-- [ClipboardManager::copyOutputToClipboard]";
        return;
    }

    QString clipboardText = "```\n";
    clipboardText += QString("%1 | %2 | %3 | %4\n")
        .arg("Name", -10)
        .arg("Role", -10)
        .arg("% of Games", -10)
        .arg("Times Played", -10);
    clipboardText += "---------------------------------------------\n";

    for (Player* player : playersList) {
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

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(clipboardText);

    qDebug() << "<-- [ClipboardManager::copyOutputToClipboard]";
}
