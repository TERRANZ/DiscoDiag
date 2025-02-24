#include "obdservice.h"
#include <qthread.h>
#include <src/command/commandIds.h>
#include <src/command/impl/displayheadercommandimpl.h>
#include <src/command/impl/obdresetfixcommandimpl.h>
#include <src/command/impl/selectprotocolobdcommandimpl.h>

#include "src/command/impl/coolanttempcommandimpl.h"

ObdService::ObdService(QObject *parent) : QObject(parent) {
    backend = new BtBackend(this);
    // commands.insert(TC_TEMP, TransferCaseTempCommandImpl());
    // commands.insert(TC_ROT_ENG, TransferCaseRotEngCommandImpl());
    // commands.insert(TC_SOL_POS, TransferCaseSolenoidPositionCommandImpl());
}

void ObdService::startService() {
    if (QList<QBluetoothAddress> adapters = BtBackend::listAdapters(); adapters.empty()) {
        qDebug() << "No adapters found";
        emit serviceError("No adapters found");
    } else {
        backend = new BtBackend(this);
        connect(backend, &BtBackend::messageReceived, this, &ObdService::messageReceived);
        connect(backend, QOverload<const QString &>::of(&BtBackend::connected), this, &ObdService::connected);
        connectionState = DEV_SELECTED;
        backend->startClient();
    }
}

void ObdService::stopService() { backend->stopClient(); }

void ObdService::messageReceived(const QString &sender,
                                 const QString &message) {
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
            qDebug() << "Connection state DEV_CONFIG => INWORK";
            connectionState = INWORK;
            break;
        case INWORK: {
            QString calculated = commands.value(sender).calculate(message);
            qDebug() << "Command " << sender
                    << " completed with result: " << calculated;
            emit updateUI(new ObdResult());
            QThread::msleep(1000);
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


void ObdService::doObdLoop() {
    doObdPreparationStep();

    if (connectionState == INWORK) {
        // sendDiscoCommands();
        backend->sendCommand(CoolantTempCommandImpl());
    }
    if (connectionState == ERROR) {
    }
}

void ObdService::doObdPreparationStep() {
    switch (connectionState) {
        case CONNECTED:
            backend->sendCommand(ObdResetFixCommandImpl());
            break;
        case RESETTED:
            backend->sendCommand(DisplayHeaderCommandImpl());
            break;
        case DEV_CONFIG:
            backend->sendCommand(SelectProtocolObdCommandImpl());
            break;
    }
}
