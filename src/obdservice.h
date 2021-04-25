#ifndef OBDSERVICE_H
#define OBDSERVICE_H

#include <QObject>
#include <QtBluetooth/qbluetoothserviceinfo.h>

#include "obdresult.h"
#include "btbackend.h"

class ObdService : public QObject
{
    Q_OBJECT
public:
    enum ConnectionState {
        NC = 0,
        DEV_SELECTED = 1,
        CONNECTED = 2,
        RESETTED = 3,
        PROTOCOL_SELECTED = 4,
        INWORK = 5,
        ERROR = -1
    };

    explicit ObdService(QObject *parent = nullptr);

    void startService();
    void stopService();

signals:
    void updateUI(ObdResult *resul);
    void serviceError(QString reason);

private:
    BtBackend *backend;
};

#endif // OBDSERVICE_H
