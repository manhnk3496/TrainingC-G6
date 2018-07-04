
#include "mainwindow.h"
#include "chatserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ChatServer w;
    w.show();

    return a.exec();
}
