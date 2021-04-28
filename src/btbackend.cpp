#include <src/command/ObdParser.h>
#include "btbackend.h"

BtBackend::BtBackend(QObject *parent) : QObject(parent) {
}

void BtBackend::startClient() {
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    QBluetoothAddress address = QBluetoothAddress("00:1D:A5:1E:C7:FB");
    socket->connectToService(address, QBluetoothUuid(QBluetoothUuid::SerialPort));
    qDebug() << "ConnectToService done";

    connect(socket, &QBluetoothSocket::readyRead, this, &BtBackend::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&BtBackend::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &BtBackend::disconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &BtBackend::onSocketErrorOccurred);
}

void BtBackend::connected() {
    emit connected(socket->peerName());
}

void BtBackend::readSocket() {
    if (!socket)
        return;

    while (socket->canReadLine()) {
        const QByteArray line = socket->readLine();
        const QString value = QString::fromUtf8(line.constData(), line.length());
        qDebug() << value;
        qDebug() << "Current stored command: " << currCmdId;
        qDebug() << "CmdId: " << ObdParser::extractCommandId(value);
        qDebug() << "DigitA: " << ObdParser::extractDigitA(value);
        qDebug() << "DigitB: " << ObdParser::extractDigitB(value);
        emit messageReceived(socket->peerName(), value);
    }
}

void BtBackend::sendCommand(const AbstractCommand &command) {
    currCmdId = command.getCmdId();
    qDebug() << "Writing command " << command.getCmdId() << " " << command.getCmdName() << " to socket";
    QByteArray text = command.getCmdId().toUtf8();
    socket->write(text);
}

void BtBackend::stopClient() {
    delete socket;
    socket = nullptr;
}

QList<QBluetoothAddress> BtBackend::listAdapters() {
    QList<QBluetoothHostInfo> devices = QBluetoothLocalDevice::allDevices();
    QList<QBluetoothAddress> result = QList<QBluetoothAddress>();
    if (!devices.empty()) {
                foreach (QBluetoothHostInfo info, devices) {
                result.append(info.address());
            }
    }
    return result;
}

void BtBackend::onSocketErrorOccurred(QBluetoothSocket::SocketError) {
}
