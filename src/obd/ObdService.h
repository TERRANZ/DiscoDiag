#ifndef OBDSERVICE_H
#define OBDSERVICE_H

#include <QThread>

#include "ObdResult.h"
#include "src/bt/BtBackend.h"
#include "src/ui/RemoteSelector.h"

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
    void updateUI(ObdResult &result);

    void serviceError(QString reason);

private slots:
    void messageReceived(const QString &sender, const QString &message);

    void connected(const QString &name);

    void processResult(AbstractCommand *cmd, const QString &message);

private:
    BtBackend *backend;
    ConnectionState connectionState = NC;
    QMap<QString, AbstractCommand *> commands = QMap<QString, AbstractCommand *>();
    int m_curr_cmd = 0;

    void doObdLoop();

    void doObdPreparationStep();

    void sendDiscoCommands();

    void processMessage(const QString &sender, const QString &message);
};

#endif // OBDSERVICE_H
