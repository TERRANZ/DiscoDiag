#ifndef OBDSERVICE_H
#define OBDSERVICE_H

#include <QObject>
#include "obdresult.h"

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
    void error(QString &reason);
};

#endif // OBDSERVICE_H
