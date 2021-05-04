#include <src/command/impl/SelectProtocolObdCommandImpl.h>
#include <src/command/impl/ObdResetFixCommandImpl.h>
#include <src/command/impl/DisplayHeaderCommandImpl.h>
#include "obdservice.h"

ObdService::ObdService(QObject *parent) : QObject(parent) {
    backend = new BtBackend(this);
}

void ObdService::startService() {
    QList<QBluetoothAddress> adapters = BtBackend::listAdapters();
    if (adapters.empty()) {
        qDebug() << "No adapters found";
        emit serviceError("No adapters found");
    } else {
        backend = new BtBackend(this);
        connect(backend, &BtBackend::messageReceived, this, &ObdService::messageReceived);
        connect(backend, QOverload<const QString &>::of(&BtBackend::connected), this, &ObdService::connected);
        connectionState = ConnectionState::DEV_SELECTED;
        backend->startClient();
    }
}

void ObdService::stopService() {
    backend->stopClient();
}

void ObdService::messageReceived(const QString &sender, const QString &message) {
    qDebug() << sender << " : " << message;
    switch (connectionState) {
        case CONNECTED:
            qDebug() << "Connection state CONNECTED => RESETTED";
            connectionState = RESETTED;
            break;
        case RESETTED:
            qDebug() << "Connection state RESETTED => DEV_CONFIG";
            connectionState = DEV_CONFIG;
            break;
        case DEV_CONFIG:
            qDebug() << "Connection state DEV_CONFIG => PROTOCOL_SELECTED";
            connectionState = PROTOCOL_SELECTED;
            break;
        case PROTOCOL_SELECTED:
            qDebug() << "Connection state PROTOCOL_SELECTED => INWORK";
            connectionState = INWORK;
            break;
        case INWORK:
            emit updateUI(new ObdResult());
            break;
        case ERROR:
            break;
    }
}

void ObdService::connected(const QString &name) {
    qDebug() << "Connected to:" << name;
    connectionState = CONNECTED;

//    startConnectionSeq();
    backend->sendCommand(ObdResetFixCommandImpl());
    backend->sendCommand(DisplayHeaderCommandImpl());
    backend->sendCommand(SelectProtocolObdCommandImpl());
    backend->sendCommand(SelectControlModuleCommandImpl());
}

void ObdService::startConnectionSeq() {
    qDebug() << "Starting OBD Connection sequence";
    bool stop = false;
    while (!stop) {
//        QThread::msleep(5000);

        doObdPreparationStep();

        if (connectionState == ConnectionState::INWORK) {
            stop = true;
        }
        if (connectionState == ConnectionState::ERROR) {
            stop = true;
        }
    }
}

void ObdService::doObdPreparationStep() {
    switch (connectionState) {
        case CONNECTED:
            backend->sendCommand(ObdResetFixCommandImpl());
            break;
        case DEV_CONFIG:
            backend->sendCommand(DisplayHeaderCommandImpl());
            break;
        case RESETTED:
            backend->sendCommand(SelectProtocolObdCommandImpl());
            break;

    }
}
