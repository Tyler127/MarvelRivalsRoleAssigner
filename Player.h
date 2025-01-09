#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QDebug>

class Player
{
public:
    // Constructor
    Player(QString &name);

    // Setters
    void setTotalGames(int games);
    void setVanguardCount(int count);
    void setDuelistCount(int count);
    void setStrategistCount(int count);
    void setAssignedRole(QString role);

    // Getters
    int getTotalGames();
    int getVanguardCount();
    int getDuelistCount();
    int getStrategistCount();
    QString getAssignedRole();
    QString getName();

    // Overload QDebug stream operator for easy printing
    friend QDebug operator<<(QDebug debug, Player &player);

    // Method to return a string representation of the player
    QString toString();

private:
    QString name;
    QString assignedRole;
    int totalGames;
    int vanguardCount;
    int duelistCount;
    int strategistCount;
};

#endif // PLAYER_H
