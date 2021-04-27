#ifndef OBDSERVICE_H
#define OBDSERVICE_H

#include <QObject>
#include <QBluetoothServiceInfo>
#include <QThread>

#include "obdresult.h"
#include "btbackend.h"
#include "remoteselector.h"

class ObdService : public QObject {
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

private slots:

    void messageReceived(const QString &sender, const QString &message);

    void connected(const QString &name);

private:
    BtBackend *backend;
    ConnectionState connectionState = ConnectionState::NC;

    void startConnectionSeq();

    void doObdPreparationStep();
};

#endif // OBDSERVICE_H
