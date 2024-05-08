#ifndef MANGER_CLIENT_H
#define MANGER_CLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

class Manger_Client : public QObject
{
    Q_OBJECT
public:
    explicit Manger_Client(QObject *parent = nullptr);

signals:
    void responseReceived(const QByteArray &responseData);

public slots:
    void get(const QString &path);
    void post(const QString &path, const QByteArray &data);

private slots:
    void readyRead();
    void encrypted(QNetworkReply *reply);
    void finished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // MANGER_CLIENT_H



