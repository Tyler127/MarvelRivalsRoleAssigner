#include "mainwindow.h"
#include "Config.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

// Global log file
QFile logFile;

void logFileMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QTextStream out(&logFile);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ") << msg << Qt::endl;
    out.flush();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // TODO: figure out how to put the config file into the assets folder instead of manually
    // having to place it into the build folder
    Config& config = Config::getInstance();
    if (!config.loadConfig("application_config.json")) {
        return -1;
    }

    if (config.getUseLogFile()) {
        // Set up the log file
        logFile.setFileName("application.log");
        if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
            qCritical() << "Unable to open log file for writing.";
            return -1;
        }

        // Install the log file message handler
        qInstallMessageHandler(logFileMessageHandler);
    }

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MarvelRivalsRoleAssigner_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // Set the application icon
    QIcon appIcon(":/assets/app_icon.ico");
    a.setWindowIcon(appIcon);

    qDebug() << "--> [main]: Application Launching";
    MainWindow w;
    w.show();
    int result = a.exec();
    qDebug() << "<-- [main]: Application Closed";

    // Close the log file
    if (config.getUseLogFile()) {
        logFile.close();
    }
    
    return result;
}
