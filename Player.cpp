#include "player.h"

Player::Player(QString &name)
    : name(name), totalGames(0), vanguardCount(0), duelistCount(0), strategistCount(0)
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

// Overload the QDebug stream operator
QDebug operator<<(QDebug debug, Player &player)
{
    debug << "Player Name: " << player.name
          << ", Total Games: " << player.totalGames
          << ", Vanguard Count: " << player.vanguardCount
          << ", Duelist Count: " << player.duelistCount
          << ", Strategist Count: " << player.strategistCount;
    return debug;
}

// toString() method for string representation of the player
QString Player::toString()
{
    return QString("Player Name: %1, Total Games: %2, Vanguard Count: %3, Duelist Count: %4, Strategist Count: %5")
    .arg(name)
        .arg(totalGames)
        .arg(vanguardCount)
        .arg(duelistCount)
        .arg(strategistCount);
}
