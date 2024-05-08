#include "manger_client.h"

Manger_Client::Manger_Client(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::encrypted, this, &Manger_Client::encrypted);
    connect(manager, &QNetworkAccessManager::finished, this, &Manger_Client::finished);
}

void Manger_Client::get(const QString &path)
{
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(path)));
    connect(reply, &QNetworkReply::readyRead, this, &Manger_Client::readyRead);
}

void Manger_Client::post(const QString &path, const QByteArray &data)
{
    QNetworkRequest request;
    request.setUrl(QUrl(path)); // Set the URL for the request

    // Set the content type header
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // Assuming data is JSON

    QNetworkReply *reply = manager->post(request, data);
    connect(reply, &QNetworkReply::readyRead, this, &Manger_Client::readyRead);
}

void Manger_Client::readyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        qWarning() << "Invalid reply object";
        return;
    }

    qDebug() << "Ready to read data from server";
    QByteArray responseData = reply->readAll();
    qDebug() << "Response from server:" << responseData;

    // Check if the response indicates correct password
    if (responseData.contains("password correct")) {
        qInfo() << "Password is correct";
        // Add your further logic here for what to do if password is correct
    } else if (responseData.contains("password incorrect")) {
        qInfo() << "Password is incorrect";
        // Add your further logic here for what to do if password is incorrect
    } else {
        qInfo() << "Unexpected response from server";
        // Add your further logic here for unexpected responses
    }

    reply->deleteLater();
}






void Manger_Client::encrypted(QNetworkReply *reply)
{
    qInfo() << "Connection is secured";
    reply->deleteLater(); // Ensure reply object is deleted to avoid memory leaks
}

void Manger_Client::finished(QNetworkReply *reply)
{
    qInfo() << "Reply finished";
    reply->deleteLater(); // Ensure reply object is deleted to avoid memory leaks
}
