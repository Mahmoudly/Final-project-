
#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QDebug>
#include <QTextStream>

extern QTcpSocket socket;
extern QByteArray response;

void Deposit_Withdraw_OtherOpertion(QTcpSocket& socket);
void NewuserName(QTcpSocket& socket);
void Account_Num(QTcpSocket& socket);
void Options_Admin(QTcpSocket& socket);
void handleServerResponse(QTcpSocket& socket);
void processAdminLogin(QTcpSocket& socket);
void processUserChoice(QTcpSocket& socket);
void UserName(QTcpSocket& socket);
void Password_User(QTcpSocket& socket);
void Amount_operation(QTcpSocket& socket);
void OtherOptions_User(QTcpSocket& socket);
void processUserChoice(QTcpSocket& socket);

#endif // CLIENT_H
