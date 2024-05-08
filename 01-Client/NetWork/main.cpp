#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include <QTextStream>

QTcpSocket socket;
QByteArray response;
void handleServerResponse(QTcpSocket& socket);
void processAdminLogin(QTcpSocket& socket);
void Options_Admin(QTcpSocket& socket);
void NewuserName(QTcpSocket& socket);
void UserName(QTcpSocket& socket);
void Password_User(QTcpSocket& socket);
void Amount_operation(QTcpSocket& socket);
void OtherOptions_User(QTcpSocket& socket);
void Deposit_Withdraw_OtherOpertion(QTcpSocket& socket);
void Account_Num(QTcpSocket& socket);
void processUserChoice(QTcpSocket& socket);

void handleServerResponse(QTcpSocket& socket) {
    if (socket.waitForReadyRead()) {
        response = socket.readAll();
        qDebug() << "Response from server:" << response;
    } else {
        qDebug() << "Error reading response from server: " << socket.errorString();
    }
}

void processAdminLogin(QTcpSocket& socket) {
    qDebug() << "Please enter admin password:";
    QTextStream input(stdin);
    QString adminPassword = input.readLine().trimmed();
    socket.write(adminPassword.toUtf8());
    socket.waitForBytesWritten();
    qDebug() << "Admin password sent to server:" << adminPassword;
    handleServerResponse(socket);

    if (response == "Incorrect admin password. Please try again.\r\n") {
        processAdminLogin(socket);
    }
    Options_Admin(socket);
}

void Options_Admin(QTcpSocket& socket) {
    qDebug() << "Please enter option 1-display data about user 2-updata acount  3-delete account 4-adduser :";
    QTextStream input(stdin);
    QString options = input.readLine().trimmed();
    socket.write(options.toUtf8());
    socket.waitForBytesWritten();
    qDebug() << "Admin option sent to server:" << options;
    handleServerResponse(socket);

    if (options == "display") {
        QTextStream input(stdin);
        QString username ;
        int counter_Try = 0 ;
        do {
            qDebug() << "Please enter username:";
            username = input.readLine().trimmed();
            socket.write(username.toUtf8());
            socket.waitForBytesWritten();
            qDebug() << "Username sent to server:" << username;
            handleServerResponse(socket);
        } while (response == "Username not  exists.to display info , try again:\r\n" && counter_Try <= 3);
        return;

    } else if (options == "updata") {
        QTextStream input(stdin);
        QString username;
        int counter_Try = 0 ;
        do {
            qDebug() << "Please enter username:";
            username = input.readLine().trimmed();
            socket.write(username.toUtf8());
            socket.waitForBytesWritten();
            qDebug() << "Username sent to server:" << username;
            handleServerResponse(socket);
        } while (response == "Username not  exists.to Updata , try again:\r\n" && counter_Try <= 3);
        NewuserName(socket);
        Account_Num(socket);
        return;
    } else if (options == "delete") {
        QTextStream input(stdin);
        QString username ;
        int counter_Try = 0 ;
        do {
            qDebug() << "Please enter username you want delete :";
            username=input.readLine().trimmed();
            socket.write(username.toUtf8());
            socket.waitForBytesWritten();
            qDebug() << "Username sent to server:" << username;
            handleServerResponse(socket);
            counter_Try++;
        } while (response == "Username not  exists.to delete , try again:\r\n" && counter_Try <= 3);
        return;
    } else if (options == "adduser") {
        NewuserName(socket);
        Account_Num(socket);
        return;
    } else {
        qInfo() << "invalid options in admin , try again please :  ";
        Options_Admin(socket);
    }
}

void NewuserName(QTcpSocket& socket) {
    qDebug() << "Please enter new username:";
    QTextStream input(stdin);
    QString newUsername = input.readLine().trimmed();
    socket.write(newUsername.toUtf8());
    socket.waitForBytesWritten();
    qDebug() << "New username sent to server:" << newUsername;
    handleServerResponse(socket);

    if (response == "Username already exists. Please enter a different username:\r\n") {
        NewuserName(socket);
    }
}

void UserName(QTcpSocket& socket) {
    qDebug() << "Please enter username:";
    QTextStream input(stdin);
    QString username = input.readLine().trimmed();
    socket.write(username.toUtf8());
    socket.waitForBytesWritten();
    qDebug() << "Username sent to server:" << username;
    handleServerResponse(socket);

    if (response == "Invalid username. Please try again.\r\n") {
        UserName(socket);
    }
}

void Password_User(QTcpSocket& socket) {
    QTextStream input(stdin);
    qDebug() << "Please enter password:";
    QString password = input.readLine().trimmed();
    socket.write(password.toUtf8());
    socket.waitForBytesWritten();
    qDebug() << "Password sent to server:" << password;
    handleServerResponse(socket);
    if (response == "Invalid account number. Please try again.\r\n") {
        Password_User(socket);
    }
}

void Amount_operation(QTcpSocket& socket) {
    QTextStream input(stdin);
    qDebug() << "Please enter the amount:";
    QString amount = input.readLine().trimmed();
    socket.write(amount.toUtf8());
    socket.waitForBytesWritten();
    qDebug() << "Amount sent to server:" << amount;
    handleServerResponse(socket);
    if (response == "Invalid withdrawal amount. Please enter a valid amount:\r\n" || response == "Invalid deposit amount. Please enter a valid amount deposit :\r\n" || response == "Insufficient balance for withdrawal. Please try again:\r\n") {
        Amount_operation(socket);
    }
}

void OtherOptions_User(QTcpSocket& socket) {
    QTextStream input(stdin);
    qDebug() << "if you want change password presss : 1  , know your balance press 2 ,to exit press 0  " ;
    QString choice  = input.readLine().trimmed().toLower();
    socket.write(choice.toUtf8());
    socket.waitForBytesWritten();
    if (choice == "1") {
        qDebug() << "Please Enter New password  : " ;
        QString newPass  = input.readLine().trimmed().toLower();
        socket.write(newPass.toUtf8());
        socket.waitForBytesWritten();
        handleServerResponse(socket);
        return;
    } else if (choice == "2") {
        qDebug() << "Please Enter ok to  view balance    : " ;
        QString newPass  = input.readLine().trimmed().toLower();
        socket.write(newPass.toUtf8());
        socket.waitForBytesWritten();
        handleServerResponse(socket);
        return;
    } else if (choice == "0") {
        Deposit_Withdraw_OtherOpertion(socket);
    } else {
        qDebug() << "your choice is not found try again : " ;
        OtherOptions_User(socket);
    }
}

void Deposit_Withdraw_OtherOpertion(QTcpSocket& socket) {
    qDebug() << "Do you want to withdraw or deposit? (withdraw/deposit/other):";
    QTextStream input(stdin);
    QString action = input.readLine().trimmed().toLower();
    socket.write(action.toUtf8());
    socket.waitForBytesWritten();
    qDebug() << "Action sent to server:" << action;
    handleServerResponse(socket);
    if (action == "other") {
        OtherOptions_User(socket);
        return;
    }
    if (response == "Invalid choice. Please try again (withdraw/deposit):\r\n") {
        Deposit_Withdraw_OtherOpertion(socket);
    } else {
        Amount_operation(socket);
    }
}

void Account_Num(QTcpSocket& socket) {
    qDebug() << "Please enter account number and blance sperated by :";
    QTextStream input(stdin);
    QString accountNumber = input.readLine().trimmed();
    socket.write(accountNumber.toUtf8());
    socket.waitForBytesWritten();
    qDebug() << "Account number sent to server:" << accountNumber;
    handleServerResponse(socket);
    if (response == "Invalid input. Please enter account number and balance separated by ':'\r\n") {
        Account_Num(socket);
    }
}

void processUserChoice(QTcpSocket& socket) {
    QByteArray data = socket.readAll();
    QTextStream input(stdin);
    qDebug() << "Do you want anything else? (yes/no):";
    QString choice = input.readLine().trimmed().toLower();

    if (choice == "yes") {
        qDebug() << "Please select login type (user/admin):";
        QString loginType = input.readLine().trimmed();
        socket.write(loginType.toUtf8());
        socket.waitForBytesWritten();
        qDebug() << "Login type sent to server:" << loginType;
        handleServerResponse(socket);

        if (loginType == "admin") {
            processAdminLogin(socket);
        } else if (loginType == "user") {
            UserName(socket);
            Password_User(socket);
            Deposit_Withdraw_OtherOpertion(socket);
        } else {
            qDebug() << "Invalid login type. Please try again.";
            processUserChoice(socket);
            return;
        }

        qDebug() << "Do you want anything else? (yes/no):";
        choice = input.readLine().trimmed().toLower();
        if (choice == "yes") {
            processUserChoice(socket);
        } else {
            socket.close();
            QCoreApplication::exit(0);
        }
    } else {
        socket.close();
        QCoreApplication::exit(0);
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    socket.connectToHost("localhost", 1234);
    if (!socket.waitForConnected()) {
        qDebug() << "Error connecting to server: " << socket.errorString();
        return 1;
    }

    processUserChoice(socket);

    return a.exec();
}
