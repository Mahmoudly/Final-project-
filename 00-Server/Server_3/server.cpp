#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent) {
    readCustomerDataFromFile();
}

void Server::RsetFlags() {
    loginTypeReceived = false;
    adminPasswordReceived = false;
    usernameReceived = false;
    newUsernameReceived = false;
    accountNumberReceived = false;
    fileInitialized = false;
    withdrawalOrDepositReceived = false;
    withdrawalAmountReceived = false;
    depositAmountReceived = false;
    otheroptionReceived = false;
    NewPassword = false;
    DataBalance = false;
    login_user = false;
    Option_Admin = false;
    updata = false;
    adduser = false;
    display = false;
    Delete = false;
    updataDate = false;
}

void Server::incomingConnection(qintptr socketDescriptor) {

    Worker *worker = new Worker(socketDescriptor);
    QThread *thread = new QThread;
    worker->moveToThread(thread);


    thread->start();
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
        QByteArray data = socket->readAll().trimmed();

        qInfo()<<"counter "<<counter++;

        if (!loginTypeReceived) {
            qDebug() << "Received login type from client:" << data;

            if (data == "user") {
                socket->write("Please enter username:\r\n");
                socket->waitForBytesWritten();
                loginType = data;
                login_user = true;
                loginTypeReceived = true;
                usernameReceived = false;
            }
            else if (data == "admin") {
                socket->write("Please enter admin password:\r\n");
                socket->waitForBytesWritten();
                loginType = data;
                loginTypeReceived = true;
                accountNumberReceived = false;
            }
            else {
                socket->write("Invalid login type. Please try again.\r\n");
                socket->waitForBytesWritten();
                // socket->close();
                return;
            }
        } else if (loginType == "admin" && !adminPasswordReceived) {
            if (data == "123456") {
                socket->write("Admin password correct. \r\n");
                socket->waitForBytesWritten();
                adminPasswordReceived = true;
                newUsernameReceived = true;
                Option_Admin = true;
            } else {
                socket->write("Incorrect admin password. Please try again.\r\n");
                socket->waitForBytesWritten();
                // socket->close();
            }
        }else if(Option_Admin)
        {
            if(data =="adduser" )
            {
                socket->write("Add user :\r\n");
                socket->waitForBytesWritten();
                Option_Admin =false ;
                adduser=true ;
            }else if (data=="updata")
            {
                socket->write("updata account :\r\n");
                socket->waitForBytesWritten();
                Option_Admin =false ;
                updata=true ;
            }else if (data=="display")
            {
                socket->write("display Account :\r\n");
                socket->waitForBytesWritten();
                Option_Admin =false ;
                display=true ;

            }else if(data=="delete")
            {
                socket->write("Delete Account :\r\n");
                socket->waitForBytesWritten();
                Option_Admin =false ;
                Delete =true ;
            }else
            {
                socket->write("select correct choice : try agian  :\r\n");
                socket->waitForBytesWritten();
                return ;
            }

        }else if(adduser){

            if (customers.contains(data)) {
                socket->write("Username already exists. Please enter a different username:\r\n");
                socket->waitForBytesWritten();
                return;
            }
            newUsername = data;
            socket->write("New username received. Please enter account number and balance separated by ':'\r\n");
            socket->waitForBytesWritten();
            newUsernameReceived = false;
            accountNumberReceived = true;
            adduser = false ;
        }else if (updata){

            if (customers.contains(data)) {
                socket->write("Username already exists.to Updata:\r\n");
                username = data;
                socket->waitForBytesWritten();
                updataDate = true ;
                updata=false ;
                return;
            }
            socket->write("Username not  exists.to Updata , try again:\r\n");
            socket->waitForBytesWritten();

        }else if(updataDate){

            QPair<QString, QString> accountData = customers.value(username);
            QString accountNumber = accountData.first;
            QString balance = accountData.second;
            customers.remove(username);

            if (customers.contains(data)) {
                socket->write("Username already exists. Please enter a different username:\r\n");
                socket->waitForBytesWritten();
                return;
            }
            newUsername = data;
            socket->write("New username received. Please enter account number and balance separated by ':'\r\n");
            socket->waitForBytesWritten();
            newUsernameReceived = false;
            accountNumberReceived = true;
            updataDate =false ;
        }else if(display){

            if (customers.contains(data)) {
                QString userData;
                socket->write("Username already exists. Displaying info:\r\n");
                username = data;
                socket->waitForBytesWritten();
                socket->write("\r\n");
                socket->waitForBytesWritten();
                QPair<QString, QString> accountData = customers.value(username);
                userData  = "Username: " + username + "\r\n";
                userData += "Account Number: " + accountData.first + "\r\n";
                userData += "Balance: " + accountData.second + "\r\n";
                socket->write(userData.toUtf8());
                socket->waitForBytesWritten();
                display = false;
                RsetFlags();
                return;
            }

            socket->write("Username not  exists.to display info , try again:\r\n");
            socket->waitForBytesWritten();


        }else if(Delete){
            if (customers.contains(data)) {
                socket->write("Username already exists.to delete:\r\n");

                socket->waitForBytesWritten();
                username = data;
                customers.remove(username);

                // Remove any related data from the file
                QFile file("customer_data.txt");
                if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                    QTextStream in(&file);
                    QStringList lines;
                    while (!in.atEnd()) {
                        QString line = in.readLine();
                        QStringList parts = line.split(":");
                        if (parts.size() == 3 && parts[0] != username) {
                            lines.append(line);
                        }
                    }
                    file.resize(0); // Clear the file
                    QTextStream out(&file);
                    for (const QString &line : lines) {
                        out << line << "\n";
                    }
                    file.close();
                }
                Delete=false ;
                RsetFlags();
                return;
            }
            socket->write("Username not  exists.to delete , try again:\r\n");
            socket->waitForBytesWritten();

        } else if (accountNumberReceived) {
            QStringList accountData = QString(data).split(':');
            if (accountData.size() != 2) {
                socket->write("Invalid input. Please enter account number and balance separated by ':'\r\n");
                socket->waitForBytesWritten();
                return;
            }
            QString accountNumber = accountData[0];
            QString balance = accountData[1];
            customers.insert(newUsername, qMakePair(accountNumber, balance));
            socket->write("New account created successfully.\r\n");
            socket->waitForBytesWritten();
            qDebug() << "New username, account number, and balance added by admin:" << newUsername << accountNumber << balance;
            if (!fileInitialized) {
                saveCustomerDataToFile("");
                fileInitialized = true;
            } else {
                saveCustomerDataToFile("");
            }

            loginTypeReceived = false;
            adminPasswordReceived = false;
            usernameReceived = true;
            newUsernameReceived = false;
            accountNumberReceived = false;
            RsetFlags();
        }
        else if (withdrawalOrDepositReceived) {

            qInfo()<<"Choice "<<data ;
            if (data == "withdraw") {
                socket->write("Enter the amount to withdraw:\r\n");
                socket->waitForBytesWritten();
                withdrawalAmountReceived = true;
                withdrawalOrDepositReceived= false ;
            } else if (data == "deposit") {
                socket->write("Enter the amount to deposit:\r\n");
                socket->waitForBytesWritten();
                depositAmountReceived = true;
                withdrawalOrDepositReceived=false;
            }else if (data=="other")
            {
                socket->write("Enter other operations :\r\n");
                socket->waitForBytesWritten();
                otheroptionReceived  = true;
                withdrawalOrDepositReceived=false;

            } else {
                socket->write("Invalid choice. Please try again (withdraw/deposit):\r\n");
                socket->waitForBytesWritten();
                // socket->close();
                return;
            }
        }else if(NewPassword==true )
        {

            QString newpassStr = data;
            bool ok;
            double newpass= newpassStr.toDouble(&ok);
            QPair<QString, QString> accountData = customers.value(username);
            double currentpass = accountData.first.toDouble();
            accountData.first = QString::number(newpass);
            customers.insert(username, accountData);
            saveCustomerDataToFile("");
            socket->write("change password successful  :\r\n");
            socket->waitForBytesWritten();
            NewPassword = false ;
            loginTypeReceived = true;
            RsetFlags();
            return ;



        }else if(DataBalance){
            QPair<QString, QString> accountData = customers.value(username);
            double currentBalance = accountData.second.toDouble();
            QByteArray balanceData = "balance : " +  QByteArray::number(currentBalance)+" $ "; // Convert double to QByteArray
            socket->write(balanceData);
            socket->waitForBytesWritten();
            DataBalance = false ;
            loginTypeReceived = false;
            RsetFlags();
            return  ;
        }
        else if (withdrawalAmountReceived) {
            QString withdrawalStr = data;
            qInfo()<<"receive amount is : "<<data ;
            bool ok;
            double withdrawal = withdrawalStr.toDouble(&ok);
            if (!ok || withdrawal <= 0) {
                socket->write("Invalid withdrawal amount. Please enter a valid amount:\r\n");
                socket->waitForBytesWritten();
                return;
            }

            QPair<QString, QString> accountData = customers.value(username);
            double currentBalance = accountData.second.toDouble();
            if (currentBalance >= withdrawal) {
                double newBalance = currentBalance - withdrawal;
                accountData.second = QString::number(newBalance);
                customers.insert(username, accountData);
                socket->write(("Withdrawal successful. New balance: " + QString::number(newBalance) + "\r\n").toUtf8());
                socket->waitForBytesWritten();
                qDebug() << "Withdrawal of" << withdrawal << "from account" << username << ". New balance:" << newBalance;

                QDateTime currentDateTime = QDateTime::currentDateTime();
                QString withdrawalDate = currentDateTime.toString(Qt::ISODate);
                QString withdrawalOperation = withdrawalDate + ": Withdrawal of " + QString::number(withdrawal) + " from account " + username + ". New balance: " + QString::number(newBalance);

                saveCustomerDataToFile(withdrawalOperation);
                loginTypeReceived = false;
                RsetFlags();
            } else {
                socket->write("Insufficient balance for withdrawal. Please try again:\r\n");
                socket->waitForBytesWritten();
                return;
            }

            withdrawalOrDepositReceived = false;
            withdrawalAmountReceived = false;
            usernameReceived = false;
        }
        else if(depositAmountReceived)
        {
            QString depositStr = data;
            qInfo()<<"receive amount of deposit is : "<<data ;
            bool ok;
            double deposit = depositStr.toDouble(&ok);
            if (!ok || deposit <= 0) {
                socket->write("Invalid deposit amount. Please enter a valid amount deposit :\r\n");
                socket->waitForBytesWritten();
                return;
            }

            QPair<QString, QString> accountData = customers.value(username);
            double currentBalance = accountData.second.toDouble();

            double newBalance = currentBalance + deposit;
            accountData.second = QString::number(newBalance);
            customers.insert(username, accountData);
            socket->write(("deposit  successful. New balance: " + QString::number(newBalance) + "\r\n").toUtf8());
            socket->waitForBytesWritten();
            qDebug() << "deposit  of" << deposit << "from account" << username << ". New balance:" << newBalance;

            QDateTime currentDateTime = QDateTime::currentDateTime();
            QString depositDate = currentDateTime.toString(Qt::ISODate);
            QString depositOperation = depositDate + ": depositDate of " + QString::number(deposit) + " from account " + username + ". New balance: " + QString::number(newBalance);
            saveCustomerDataToFile(depositOperation);
            depositAmountReceived= false ;
            loginTypeReceived = false;
            RsetFlags();
        }
        else if(otheroptionReceived==true)
        {
            if(data =="1")
            {
                NewPassword = true ;
                otheroptionReceived= false ;
                return  ;

            }
            else if (data=="2")
            {

                DataBalance = true ;
                otheroptionReceived= false ;
                return ;
            }



        }

        else if (!usernameReceived) {
            qDebug() << "Received username from client:" << data;

            if (customers.contains(data)) {
                socket->write("Please enter account number:\r\n");
                socket->waitForBytesWritten();
                username = data;
                usernameReceived = true;
            } else {
                socket->write("Invalid username. Please try again.\r\n");
                socket->waitForBytesWritten();
                // socket->close();
                return;
            }
        } else if(login_user) {
            qDebug() << "Received account number from client:" << data;

            QPair<QString, QString> accountData = customers.value(username);
            QString accountNumber = accountData.first;
            QString balance = accountData.second;
            if ( data  != accountNumber ) {
                socket->write("Invalid account number. Please try again.\r\n");
                socket->waitForBytesWritten();
                // socket->close();
                return;
            }

            socket->write("Account number correct\r\n");
            socket->waitForBytesWritten();
            qDebug() << "Welcome to the " << loginType << " page";
            withdrawalOrDepositReceived = true;
            login_user = false ;
        }



    });

    connect(socket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
}

void Server::readCustomerDataFromFile() {
    QFile file("customer_data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(":");
            if (parts.size() == 3) {
                customers.insert(parts[0], qMakePair(parts[1], parts[2]));
            }
        }
        file.close();
        fileInitialized = true;
    }
}

void Server::saveCustomerDataToFile(const QString &withdrawalOperation) {
    QFile file("customer_data.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto it = customers.begin(); it != customers.end(); ++it) {
            out << it.key() << ":" << it.value().first << ":" << it.value().second << "\n";

            if (it.key() == username) {
                file.close();
                if (file.open(QIODevice::Append | QIODevice::Text))
                    out << "\n";
                out << withdrawalOperation << "\n";
                out << "\n";
            }
        }
        file.close();
    }
}

void Server::clientDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket)
        return;

    qDebug() << "Client disconnected";
    socket->deleteLater();
}
