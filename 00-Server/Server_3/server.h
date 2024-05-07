#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QPair>
#include <QDateTime>
#include <QThread>


class Worker : public QObject {
    Q_OBJECT
public:
    Worker(qintptr socketDescriptor, QObject *parent = nullptr)
        : QObject(parent), socketDescriptor(socketDescriptor) {}

public slots:
    void process() {
        QTcpSocket socket;
        if (!socket.setSocketDescriptor(socketDescriptor)) {
            emit error(socket.error());
            return;
        }

        socket.close();
    }

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    qintptr socketDescriptor;
};

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = nullptr);

    void RsetFlags(void);

protected:
    int counter = 0;
    void incomingConnection(qintptr socketDescriptor) override;

private:
    bool loginTypeReceived = false;
    bool adminPasswordReceived = false;
    bool usernameReceived = false;
    bool newUsernameReceived = false;
    bool accountNumberReceived = false;
    bool fileInitialized = false;
    bool withdrawalOrDepositReceived = false;
    bool withdrawalAmountReceived = false;
    bool depositAmountReceived = false;
    bool otheroptionReceived = false;
    bool NewPassword = false;
    bool DataBalance = false;
    bool login_user = false;
    bool Option_Admin = false;
    bool updata = false;
    bool adduser = false;
    bool display = false;
    bool Delete = false;
    bool updataDate = false;
    QString loginType;
    QString newUsername;
    QString username;
    QMap<QString, QPair<QString, QString>> customers;

    void readCustomerDataFromFile();
    void saveCustomerDataToFile(const QString &withdrawalOperation);

private slots:
    void clientDisconnected();
};

#endif // SERVER_H
