#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "LineFollower_" + QLocale(locale).name();
        if (translator.load("/home/kamil/Documents/projects-repos/Line_Follower_ZACZ/zacz_QT/LineFollower_en_150.qm")) {
            a.installTranslator(&translator);
            qDebug() << "Translation file loaded successfully.";
        } else {
            qDebug() << "Failed to load translation file.";
        }

    }
    MainWindow w;
    w.show();
    return a.exec();
}
