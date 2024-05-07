#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;

    if (!server.listen(QHostAddress::Any, 1234)) {
        qDebug() << "Server could not start!";
        return 1;

    qDebug() << "Server started. Listening on port 1234...";
}
    return a.exec();
}



