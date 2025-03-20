#include "chatapp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatApp w;
    w.show();
    return a.exec();
}
