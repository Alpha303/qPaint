#include <QApplication>
#include "mainwindow.h"

int main (int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QIcon appIcon = QIcon(":/icons/gfx/icons/icon.png");
    QFile styleSheet(":/css/css/style.css");

    styleSheet.open(QFile::ReadOnly);
    QString style(styleSheet.readAll());
    a.setStyleSheet(style);
    a.setWindowIcon(appIcon);
    w.show();
    return a.exec();
}
