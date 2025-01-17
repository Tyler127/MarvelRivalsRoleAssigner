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

private slots:
    void createNewCSVFile();
    void openCSVFile();
    void assignRoles();
    void showAboutDialog();
    void saveCSVFile();
    void copyOutputToClipboard();

private:
    Ui::MainWindow *ui;
    QFile openFile;
    void processCSVFile(QFile* file, QString fileName);
    void setupUIFromCSV(QList<QStringList> fileData);
};

#endif // MAINWINDOW_H
