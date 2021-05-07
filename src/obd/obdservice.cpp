#include <src/command/impl/SelectProtocolObdCommandImpl.h>
#include <src/command/impl/ObdResetFixCommandImpl.h>
#include <src/command/impl/DisplayHeaderCommandImpl.h>
#include <src/command/impl/SelectControlModuleCommandImpl.h>
#include <src/command/impl/disco3/TransferCaseTempCommandImpl.h>
#include <src/command/commandIds.h>
#include <src/command/impl/disco3/TransferCaseRotEngCommandImpl.h>
#include <src/command/impl/disco3/TransferCaseSolenoidPositionCommandImpl.h>
#include "obdservice.h"

ObdService::ObdService(QObject *parent) : QObject(parent) {
    backend = new BtBackend(this);
    commands.insert(TC_TEMP, TransferCaseTempCommandImpl());
    commands.insert(TC_ROT_ENG, TransferCaseRotEngCommandImpl());
    commands.insert(TC_SOL_POS, TransferCaseSolenoidPositionCommandImpl());
}

void ObdService::startService() {
    QList<QBluetoothAddress> adapters = BtBackend::listAdapters();
    if (adapters.empty()) {
        qDebug() << "No adapters found";
        emit serviceError("No adapters found");
    } else {
        backend = new BtBackend(this);
        auto *thread = new QThread;
        backend->moveToThread(thread);
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

    processMessage(sender, message);

    doObdLoop();
}

void ObdService::processMessage(const QString &sender, const QString &message) {
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
        case INWORK: {
            QString calculated = commands.value(sender).calculate(message);
            qDebug() << "Command " << sender << " completed with result: " << calculated;
            emit updateUI(new ObdResult());
        }
            break;
        case ERROR:
            break;
    }
}

void ObdService::connected(const QString &name) {
    qDebug() << "Connected to:" << name;
    connectionState = CONNECTED;

    doObdLoop();
}

void ObdService::sendDiscoCommands() {
    backend->sendCommand(
            SelectControlModuleCommandImpl(TRANSFER_CASE_CONTROL_MODULE, "Change CM to Transfer Case"));
    backend->sendCommand(commands.value(TC_TEMP));
    backend->sendCommand(commands.value(TC_ROT_ENG));
    backend->sendCommand(commands.value(TC_SOL_POS));
}

void ObdService::doObdLoop() {
    doObdPreparationStep();

    if (connectionState == ConnectionState::INWORK) {
//        sendDiscoCommands();
    }
    if (connectionState == ConnectionState::ERROR) {
        return;
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
