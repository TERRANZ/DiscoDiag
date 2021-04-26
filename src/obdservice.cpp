#include "obdservice.h"

ObdService::ObdService(QObject *parent) : QObject(parent)
{
    backend = new BtBackend(this);
}

void ObdService::startService()
{
    QList<QBluetoothAddress> adapters =  backend->listAdapters();
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
            backend = new BtBackend(this);
            connect(backend, &BtBackend::messageReceived, this, &ObdService::messageReceived);
            backend->startClient(service);
        }
    }
}

void ObdService::stopService()
{
    backend->stopClient();
}

void ObdService::messageReceived(const QString &sender, const QString &message) {
    qDebug() << sender << " : " << message;
}
