#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QClipboard>
#include <QTimer>
#include <random>
#include "FileManager.h"
#include "RoleAssigner.h"
#include "ClipboardManager.h"
#include "Config.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void assignRolesSignal(Ui::MainWindow* ui, QList<Player*>& playersList);
    void copyOutputToClipboardSignal(QList<Player*>& playersList);

private slots:
    void createNewCSVFile();
    void openCSVFile();
    void assignRoles();
    void showAboutDialog();
    void saveCSVFile();
    void copyOutputToClipboard();
    void showNoResultsToCopyNotification();

private:
    Ui::MainWindow *ui;
    QFile openFile;
    RoleAssigner* roleAssigner;
    ClipboardManager* clipboardManager;
    void processCSVFile(QFile* file, QString fileName);
    void setupUIFromCSV(QList<QStringList> fileData);
};

#endif // MAINWINDOW_H
