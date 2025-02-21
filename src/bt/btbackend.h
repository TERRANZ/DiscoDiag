#ifndef BTBACKEND_H
#define BTBACKEND_H

#include <QBluetoothAddress>
#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QList>
#include <QObject>

#include "src/command/abstractcommand.h"

class BtBackend : public QObject {
    Q_OBJECT

public:
    explicit BtBackend(QObject *parent = nullptr);

    void startClient();

    void stopClient();

    static QList<QBluetoothAddress> listAdapters();

public slots:
    void sendCommand(const AbstractCommand &command);

signals:
    void messageReceived(const QString &cmdId, const QString &message);

    void connected(const QString &name);

    void disconnected();

    void socketErrorOccurred(const QString &errorString);

private slots:
    void readSocket();

    void connected();

    void onSocketErrorOccurred(QBluetoothSocket::SocketError error);

private:
    QBluetoothSocket *socket = nullptr;
    QString currCmdId = "";
    QByteArray line = QByteArray();
    bool readComplete = false;
};

#endif // BTBACKEND_H
