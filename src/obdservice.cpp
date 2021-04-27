#include "src/command/impl/obdresetfixcommandimpl.h"
#include "obdservice.h"

ObdService::ObdService(QObject *parent) : QObject(parent) {
    backend = new BtBackend(this);
}

void ObdService::startService() {
    QList<QBluetoothAddress> adapters = backend->listAdapters();
    if (adapters.size() == 0) {
        qDebug() << "No adapters found";
        emit serviceError("No adapters found");
    } else {
        RemoteSelector remoteSelector(adapters.at(0));
        remoteSelector.startDiscovery();
        if (remoteSelector.exec() == QDialog::Accepted) {
            QBluetoothServiceInfo service = remoteSelector.service();
            qDebug() << "Connecting to service 2" << service.serviceName()
                     << "on" << service.device().name();
            qDebug() << "Device uuid " << service.serviceUuid();
            backend = new BtBackend(this);
            connect(backend, &BtBackend::messageReceived, this, &ObdService::messageReceived);
            connect(backend, QOverload<const QString &>::of(&BtBackend::connected), this, &ObdService::connected);
            connectionState = ConnectionState::DEV_SELECTED;
            backend->startClient(service);
        }
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
