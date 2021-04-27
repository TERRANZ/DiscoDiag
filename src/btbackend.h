#ifndef BTBACKEND_H
#define BTBACKEND_H

#include <QObject>
#include <QList>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothAddress>

#include "command/abstractcommand.h"

class BtBackend : public QObject {
Q_OBJECT
public:
    explicit BtBackend(QObject *parent = nullptr);

    void startClient(const QBluetoothServiceInfo &remoteService);

    void stopClient();

    QList<QBluetoothAddress> listAdapters();

public slots:

    void sendCommand(const AbstractCommand &commmand);

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
