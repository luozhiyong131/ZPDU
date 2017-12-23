#include <QCoreApplication>
#include "initclass.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    new InitClass();

    return a.exec();
}
