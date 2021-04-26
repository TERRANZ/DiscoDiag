#include "btbackend.h"

BtBackend::BtBackend(QObject *parent) : QObject(parent)
{
}

void BtBackend::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(remoteService);
    qDebug() << "ConnectToService done";

    connect(socket, &QBluetoothSocket::readyRead, this, &BtBackend::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&BtBackend::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &BtBackend::disconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &BtBackend::onSocketErrorOccurred);
}

void BtBackend::connected()
{
    emit connected(socket->peerName());
}

void BtBackend::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine())
    {
        QByteArray line = socket->readLine();
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}

void BtBackend::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
}

void BtBackend::stopClient()
{
    delete socket;
    socket = nullptr;
}

QList<QBluetoothAddress> BtBackend::listAdapters()
{
    QList<QBluetoothHostInfo> devices = QBluetoothLocalDevice::allDevices();
    QList<QBluetoothAddress> result = QList<QBluetoothAddress>();
    if (devices.size() > 0)
    {
        foreach (QBluetoothHostInfo info, devices)
        {
            result.append(info.address());
        }
    }
    return result;
}

void BtBackend::onSocketErrorOccurred(QBluetoothSocket::SocketError)
{
}
