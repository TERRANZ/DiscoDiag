#include "src/command/impl/ObdResetFixCommandImpl.h"
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
}

void ObdService::connected(const QString &name) {
    qDebug() << "Connected";
    connectionState = ConnectionState::CONNECTED;

    startConnectionSeq();
}

void ObdService::startConnectionSeq() {
    qDebug() << "Starting OBD Connection sequence";
    bool stop = false;
    while (!stop) {
        QThread::msleep(500);

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
    backend->sendCommand(ObdResetFixCommandImpl());
}
