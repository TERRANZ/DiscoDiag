#ifndef OBDSERVICE_H
#define OBDSERVICE_H

#include <QBluetoothAddress>
#include <QObject>
#include <QTimer>

#include <memory>
#include <vector>

#include "ObdResult.h"
#include "src/command/AbstractCommand.h"

class BtBackend;

class ObdService : public QObject {
  Q_OBJECT

public:
  enum ConnectionState {
    NC = 0,
    DEV_SELECTED = 1,
    DEV_CONFIG = 2,
    CONNECTED = 3,
    RESETTED = 4,
    INWORK = 6,
    ERROR = -1
  };

  explicit ObdService(QObject *parent = nullptr);

  ~ObdService() override;

  bool startService(const QBluetoothAddress &address);

  void stopService();

  bool isRunning() const;

signals:
  void updateUI(const ObdResult &result);

  void serviceError(const QString &reason);

  void runningChanged(bool running);

private slots:
  void messageReceived(const QString &message);

  void connected(const QString &name);

  void onDisconnected();

  void onSocketError(const QString &reason);

  void sendNextCommand();

private:
  void processMessage(const QString &message);

  void processResult(AbstractCommand *cmd, const QString &message);

  bool sendCommand(const AbstractCommand &command);

  bool expectOk(const QString &message);

  void fail(const QString &reason);

  BtBackend *m_backend = nullptr;
  ConnectionState m_connectionState = NC;
  std::vector<std::unique_ptr<AbstractCommand>> m_commands;
  int m_currCmd = 0;
  AbstractCommand *m_pendingCommand = nullptr;
  ObdResult m_result;
  QTimer *m_pollTimer = nullptr;
};

#endif // OBDSERVICE_H
