#ifndef BTBACKEND_H
#define BTBACKEND_H

#include <QBluetoothAddress>
#include <QBluetoothLocalDevice>
#include <QBluetoothSocket>
#include <QList>
#include <QObject>
#include <QString>

class AbstractCommand;

class BtBackend : public QObject {
    Q_OBJECT

public:
    explicit BtBackend(QObject *parent = nullptr);

    bool startClient(const QBluetoothAddress &address);

    void stopClient();

    bool isConnected() const;

    bool sendCommand(const AbstractCommand *command);

    static QList<QBluetoothAddress> listAdapters();

signals:
    void messageReceived(const QString &message);

    void connected(const QString &name);

    void disconnected();

    void socketErrorOccurred(const QString &errorString);

private slots:
    void readSocket();

    void onConnected();

    void onDisconnected();

    void onSocketErrorOccurred(QBluetoothSocket::SocketError error);

private:
    QBluetoothSocket *m_socket = nullptr;
    QString m_buffer;
};

#endif // BTBACKEND_H
