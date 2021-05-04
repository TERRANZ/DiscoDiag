#ifndef OBDSERVICE_H
#define OBDSERVICE_H

#include <QObject>
#include <QBluetoothServiceInfo>
#include <QThread>

#include "obdresult.h"
#include "src/bt/btbackend.h"
#include "src/ui/remoteselector.h"

class ObdService : public QObject {
Q_OBJECT
public:
    enum ConnectionState {
        NC = 0,
        DEV_SELECTED = 1,
        DEV_CONFIG = 2,
        CONNECTED = 3,
        RESETTED = 4,
        PROTOCOL_SELECTED = 5,
        INWORK = 6,
        ERROR = -1
    };

    explicit ObdService(QObject *parent = nullptr);

    void startService();

    void stopService();

signals:

    void updateUI(ObdResult *result);

    void serviceError(QString reason);

private slots:

    void messageReceived(const QString &sender, const QString &message);

    void connected(const QString &name);

private:
    BtBackend *backend;
    ConnectionState connectionState = ConnectionState::NC;

    void startConnectionSeq();

    void doObdPreparationStep();

    void doObdLoop();
};

#endif // OBDSERVICE_H
