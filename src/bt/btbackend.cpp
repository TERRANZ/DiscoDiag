#include "btbackend.h"
#include <cstdio>
#include <qcontainerfwd.h>
#include <qlogging.h>

BtBackend::BtBackend(QObject *parent) : QObject(parent) {
}

void BtBackend::startClient() {
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    const auto address = QBluetoothAddress("10:21:3E:4A:3E:41");
    socket->connectToService(
        address, QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));
    qDebug() << "ConnectToService done";

    connect(socket, &QBluetoothSocket::readyRead, this, &BtBackend::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&BtBackend::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &BtBackend::disconnected);
    connect(socket, &QBluetoothSocket::errorOccurred, this, &BtBackend::onSocketErrorOccurred);
}

void BtBackend::connected() {
    emit connected(socket->peerName());
}

void BtBackend::readSocket() {
    if (!socket)
        return;
    while (socket->bytesAvailable() > 0) {
        line.append(socket->readAll().replace("\r", " "));
        // const auto value = QString::fromLatin1(line.constData(), line.length());
        // qDebug() << "Received from obd: " << value;
        // qDebug() << "Hex: " << line.toHex();
        // printf(value.toLatin1());
        // printf(line.toHex());
        if (line.endsWith(">")) {
            readComplete = true;
        } else {
            // qDebug() << "Read is not complete, current buffer: " << line;
        }
    }
    //
    if (readComplete) {
        // const auto received = QString::fromLatin1(line.constData(), line.length());
        qDebug() << "Read complete: " << line;
        emit messageReceived(currCmdId, line);
        line.clear();
        readComplete = false;
    }
}

void BtBackend::sendCommand(const AbstractCommand *command) {
    currCmdId = command->getCmdId();
    // qDebug() << "Writing command " << command.getCmdId() << " " << command.getCmdName() << " to socket";
    const auto cmd = QString(command->getCmdId() + "\r");
    const auto written = socket->write(cmd.toUtf8());
    // qDebug() << "Written " << written << " bytes to socket";
}

void BtBackend::stopClient() {
    socket->disconnectFromService();
    delete socket;
    socket = nullptr;
}

QList<QBluetoothAddress> BtBackend::listAdapters() {
    const auto devices = QBluetoothLocalDevice::allDevices();
    auto result = QList<QBluetoothAddress>();
    if (!devices.empty()) {
        foreach(QBluetoothHostInfo info, devices) {
            result.append(info.address());
        }
    }
    return result;
}

void BtBackend::onSocketErrorOccurred(const QBluetoothSocket::SocketError error) {
    qDebug() << "ERROR" << error;
}
