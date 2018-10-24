#include "chatclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client w;
    w.setWindowIcon(QIcon("octopus.png"));
    w.show();

    return a.exec();
}
