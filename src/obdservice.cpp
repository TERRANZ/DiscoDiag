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
        foreach (QBluetoothAddress addr, adapters) {
            qDebug() << addr.toString();
        }
    }
}

void ObdService::stopService()
{

}

