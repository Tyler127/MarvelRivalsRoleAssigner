#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <QObject>
#include <QList>
#include <QClipboard>
#include <QGuiApplication>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include "Player.h"

class ClipboardManager : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardManager(QObject *parent = nullptr);

public slots:
    void copyOutputToClipboard(QList<Player*>& playersList);

signals:
    void noResultsToCopy();

};

#endif // CLIPBOARDMANAGER_H
