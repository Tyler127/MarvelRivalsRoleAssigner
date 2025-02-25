#include "player.h"

Player::Player(QString name)
    : name(name), totalGames(0), vanguardCount(0), duelistCount(0), strategistCount(0), assignedRole("None")
{}

void Player::setTotalGames(int games)
{
    totalGames = games;
}

void Player::setVanguardCount(int count)
{
    vanguardCount = count;
}

void Player::setDuelistCount(int count)
{
    duelistCount = count;
}

void Player::setStrategistCount(int count)
{
    strategistCount = count;
}

void Player::setAssignedRole(QString newRole) {
    assignedRole = newRole;
}

int Player::getTotalGames()
{
    return totalGames;
}

int Player::getVanguardCount()
{
    return vanguardCount;
}

int Player::getDuelistCount()
{
    return duelistCount;
}

int Player::getStrategistCount()
{
    return strategistCount;
}

QString Player::getName()
{
    return name;
}

QString Player::getAssignedRole()
{
    return assignedRole;
}

// Overload the QDebug stream operator
QDebug operator<<(QDebug debug, Player &player)
{
    debug << "Player Name: " << player.name
          << ", Total Games: " << player.totalGames
          << ", Vanguard Count: " << player.vanguardCount
          << ", Duelist Count: " << player.duelistCount
          << ", Strategist Count: " << player.strategistCount
          << ", Assigned Role: " << player.assignedRole;
    return debug;
}

// toString() method for string representation of the player
QString Player::toString()
{
    return QString("Player Name: %1, Total Games: %2, Vanguard Count: %3, Duelist Count: %4, Strategist Count: %5, Assigned Role: %6")
    .arg(name)
        .arg(totalGames)
        .arg(vanguardCount)
        .arg(duelistCount)
        .arg(strategistCount)
        .arg(assignedRole);
}

QString Player::toCSVString() const {
    return QString("%1,%2,%3,%4,%5")
        .arg(name)
        .arg(totalGames)
        .arg(vanguardCount)
        .arg(duelistCount)
        .arg(strategistCount);
}
