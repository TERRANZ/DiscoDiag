#include "BtBackend.h"

#include "src/command/AbstractCommand.h"

#include <QDebug>

namespace {
constexpr int MAX_BUFFER_SIZE = 64 * 1024;
}

BtBackend::BtBackend(QObject *parent) : QObject(parent) {
}

bool BtBackend::startClient(const QBluetoothAddress &address) {
    if (m_socket != nullptr) {
        return false;
    }
    if (address.isNull()) {
        emit socketErrorOccurred(tr("Bluetooth address is not configured"));
        return false;
    }

    m_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(m_socket, &QBluetoothSocket::readyRead, this, &BtBackend::readSocket);
    connect(m_socket, &QBluetoothSocket::connected, this, &BtBackend::onConnected);
    connect(m_socket, &QBluetoothSocket::disconnected, this, &BtBackend::onDisconnected);
    connect(m_socket, &QBluetoothSocket::errorOccurred, this, &BtBackend::onSocketErrorOccurred);

    m_socket->connectToService(
        address, QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));
    return true;
}

void BtBackend::stopClient() {
    if (m_socket == nullptr) {
        return;
    }

    // Disconnect our slots first so that aborting the socket cannot feed a
    // stale disconnection/error back into a service that is shutting down.
    m_socket->disconnect(this);
    if (m_socket->state() != QBluetoothSocket::SocketState::UnconnectedState) {
        m_socket->abort();
    }
    m_socket->deleteLater();
    m_socket = nullptr;
    m_buffer.clear();
}

bool BtBackend::isConnected() const {
    return m_socket != nullptr &&
           m_socket->state() == QBluetoothSocket::SocketState::ConnectedState;
}

bool BtBackend::sendCommand(const AbstractCommand *command) {
    if (command == nullptr) {
        return false;
    }
    if (!isConnected()) {
        emit socketErrorOccurred(tr("Cannot send command: Bluetooth socket is not connected"));
        return false;
    }

    const QByteArray payload = command->getCmdId().toLatin1() + '\r';
    const qint64 written = m_socket->write(payload);
    if (written != payload.size()) {
        qWarning() << "Failed to write command" << command->getCmdId()
                   << "written:" << written << "expected:" << payload.size();
        return false;
    }
    return true;
}

void BtBackend::readSocket() {
    if (m_socket == nullptr) {
        return;
    }

    m_buffer.append(QString::fromLatin1(m_socket->readAll()));
    if (m_buffer.size() > MAX_BUFFER_SIZE) {
        qWarning() << "Bluetooth receive buffer overflow, dropping data";
        m_buffer.clear();
        return;
    }

    // The ELM327 family terminates every response with '>'. There may be
    // several responses in a single readyRead() delivery.
    int end = -1;
    while ((end = m_buffer.indexOf(QLatin1Char('>'))) >= 0) {
        QString frame = m_buffer.left(end);
        m_buffer.remove(0, end + 1);
        frame = frame.simplified();
        if (!frame.isEmpty()) {
            qDebug() << "Read frame:" << frame;
            emit messageReceived(frame);
        }
    }
}

void BtBackend::onConnected() {
    QString name;
    if (m_socket != nullptr) {
        name = m_socket->peerName();
        if (name.isEmpty()) {
            name = m_socket->peerAddress().toString();
        }
    }
    emit connected(name);
}

void BtBackend::onDisconnected() {
    emit disconnected();
}

void BtBackend::onSocketErrorOccurred(const QBluetoothSocket::SocketError error) {
    const QString reason = m_socket != nullptr
                               ? m_socket->errorString()
                               : tr("Unknown Bluetooth socket error");
    qWarning() << "Bluetooth socket error" << static_cast<int>(error) << reason;
    emit socketErrorOccurred(reason);
}

QList<QBluetoothAddress> BtBackend::listAdapters() {
    const auto devices = QBluetoothLocalDevice::allDevices();
    auto result = QList<QBluetoothAddress>();
    for (const auto &info : devices) {
        result.append(info.address());
    }
    return result;
}
