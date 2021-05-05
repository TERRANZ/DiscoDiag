#ifndef BTBACKEND_H
#define BTBACKEND_H

#include <QObject>
#include <QList>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothAddress>

#include "src/command/abstractcommand.h"

class BtBackend : public QObject {
Q_OBJECT
public:
    explicit BtBackend(QObject *parent = nullptr);

    void startClient();

    void stopClient();

    static QList<QBluetoothAddress> listAdapters();

public slots:

    void sendCommand(const AbstractCommand &commmand);

signals:

    void messageReceived(const QString &cmdId, const QString &message);

    void connected(const QString &name);

    void disconnected();

    void socketErrorOccurred(const QString &errorString);

private slots:

    void readSocket();

    void connected();

    void onSocketErrorOccurred(QBluetoothSocket::SocketError);

private:
    QBluetoothSocket *socket = nullptr;
    QString currCmdId = "";
};

#endif // BTBACKEND_H
