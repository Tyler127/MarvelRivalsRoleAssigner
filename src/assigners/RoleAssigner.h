#ifndef ROLEASSIGNER_H
#define ROLEASSIGNER_H

#include <QList>
#include <QSet>
#include <random>
#include <QObject>
#include "Player.h"
#include "./ui_mainwindow.h"

class RoleAssigner : public QObject
{
    Q_OBJECT

public:
    RoleAssigner(QObject* parent = nullptr);

public slots:
    void assignRoles(Ui::MainWindow* ui, QList<Player*>& playersList);

private:
    void shufflePlayers(QList<Player*>& playersList);
    void assignHandpickedRoles(QList<Player*>& playersList, int numPlayersToHandpick, int& vanguardAssignments, int& duelistAssignments, int& strategistAssignments);
    void assignRandomRoles(QList<Player*>& playersList, int numPlayersToHandpick, int& vanguardAssignments, int& duelistAssignments, int& strategistAssignments);
};

#endif // ROLEASSIGNER_H
