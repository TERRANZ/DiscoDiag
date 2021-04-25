#ifndef BTBACKEND_H
#define BTBACKEND_H

#include <QObject>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>

#include "command/abstractcommand.h"

class BtBackend : public QObject
{
    Q_OBJECT
public:
    explicit BtBackend(QObject *parent = nullptr);

    void startClient(const QBluetoothServiceInfo &remoteService);
    void stopClient();

public slots:
    void sendMessage(const QString &message);

signals:
    void messageReceived(const QString &sender, const QString &message);
    void connected(const QString &name);
    void disconnected();
    void socketErrorOccurred(const QString &errorString);

private slots:
    void readSocket();
    void connected();
    void onSocketErrorOccurred(QBluetoothSocket::SocketError);

private:
    QBluetoothSocket *socket = nullptr;
};

#endif // BTBACKEND_H
