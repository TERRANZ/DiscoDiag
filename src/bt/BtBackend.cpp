#include "BtBackend.h"
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
    socket->connectToService(address, QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));
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
        if (line.endsWith(">")) {
            readComplete = true;
        }
    }
    if (readComplete) {
        qDebug() << "Read complete: " << line;
        emit messageReceived(line);
        line.clear();
        readComplete = false;
    }
}

void BtBackend::sendCommand(const AbstractCommand *command) const {
    if (!command) {
        return;
    }
    const auto cmd = QString(command->getCmdId() + "\r");
    const auto written = socket->write(cmd.toUtf8());
    qDebug() << "Written " << written << " bytes to socket";
}

void BtBackend::stopClient() {
    if (socket != nullptr) {
        socket->disconnectFromService();
        delete socket;
        socket = nullptr;
    }
}

QList<QBluetoothAddress> BtBackend::listAdapters() {
    const auto devices = QBluetoothLocalDevice::allDevices();
    auto result = QList<QBluetoothAddress>();
    if (!devices.empty()) {
        foreach(const auto info, devices) {
            result.append(info.address());
        }
    }
    return result;
}

void BtBackend::onSocketErrorOccurred(
    const QBluetoothSocket::SocketError error) {
    qDebug() << "ERROR" << error;
}
