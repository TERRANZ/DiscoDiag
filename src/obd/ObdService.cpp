#include "ObdService.h"

#include <QDebug>

#include "src/bt/BtBackend.h"
#include "src/command/commandIds.h"
#include "src/command/impl/common/DisplayHeaderCommandImpl.h"
#include "src/command/impl/common/ObdResetFixCommandImpl.h"
#include "src/command/impl/common/SelectProtocolObdCommandImpl.h"
#include "src/command/impl/lr322/OilLevelCommandImpl.h"
#include "src/command/impl/lr322/OilVolumeCommandImpl.h"
#include "src/command/impl/lr322/temperature/AmbientAirTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/ComprTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/CoolantTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/GbTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/IntakeAirTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/OilTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/RDTempCommandImpl.h"
#include "src/command/impl/lr322/temperature/TCTempCommandImpl.h"
#include "src/obd/ObdParser.h"

namespace {
constexpr int POLL_INTERVAL_MS = 100;
}

ObdService::ObdService(QObject *parent)
    : QObject(parent), m_backend(new BtBackend(this)) {
  m_commands.emplace_back(std::make_unique<CoolantTempCommandImpl>());
  m_commands.emplace_back(std::make_unique<OilTempCommandImpl>());
  m_commands.emplace_back(std::make_unique<GbTempCommandImpl>());
  m_commands.emplace_back(std::make_unique<ComprTempCommandImpl>());
  m_commands.emplace_back(std::make_unique<AmbientAirTempCommandImpl>());
  m_commands.emplace_back(std::make_unique<IntakeAirTempCommandImpl>());
  m_commands.emplace_back(std::make_unique<TCTempCommandImpl>());
  m_commands.emplace_back(std::make_unique<RDTempCommandImpl>());
  m_commands.emplace_back(std::make_unique<OilLevelCommandImpl>());
  m_commands.emplace_back(std::make_unique<OilVolumeCommandImpl>());

  connect(m_backend, &BtBackend::messageReceived, this,
          &ObdService::messageReceived);
  connect(m_backend, &BtBackend::connected, this, &ObdService::connected);
  connect(m_backend, &BtBackend::disconnected, this,
          &ObdService::onDisconnected);
  connect(m_backend, &BtBackend::socketErrorOccurred, this,
          &ObdService::onSocketError);

  m_pollTimer.setSingleShot(true);
  m_pollTimer.setInterval(POLL_INTERVAL_MS);
  connect(&m_pollTimer, &QTimer::timeout, this, &ObdService::sendNextCommand);
}

ObdService::~ObdService() = default;

bool ObdService::startService(const QBluetoothAddress &address) {
  if (isRunning()) {
    emit serviceError(tr("Diagnostics is already running"));
    return false;
  }
  if (address.isNull()) {
    emit serviceError(tr("Bluetooth device address is not configured"));
    return false;
  }
  if (BtBackend::listAdapters().isEmpty()) {
    emit serviceError(tr("No Bluetooth adapters found"));
    return false;
  }

  if (m_connectionState == ERROR) {
    stopService();
  }

  m_pollTimer.stop();
  m_pendingCommand = nullptr;
  m_currCmd = 0;
  m_result = ObdResult();
  m_connectionState = DEV_SELECTED;

  if (!m_backend->startClient(address)) {
    m_connectionState = NC;
    return false;
  }

  emit updateUI(m_result);
  return true;
}

void ObdService::stopService() {
  m_pollTimer.stop();
  m_pendingCommand = nullptr;
  m_connectionState = NC;
  m_backend->stopClient();
}

bool ObdService::isRunning() const {
  return m_connectionState != NC && m_connectionState != ERROR;
}

void ObdService::connected(const QString &name) {
  if (m_connectionState != DEV_SELECTED) {
    qWarning() << "Unexpected Bluetooth connection while in state"
               << m_connectionState;
    stopService();
    return;
  }

  qDebug() << "Connected to:" << name;
  m_connectionState = CONNECTED;

  ObdResetFixCommandImpl resetCommand;
  sendCommand(resetCommand);
}

void ObdService::messageReceived(const QString &message) {
  qDebug() << "Received message" << message;
  processMessage(message);
}

void ObdService::processMessage(const QString &message) {
  switch (m_connectionState) {
    case CONNECTED: {
      if (!expectOk(message)) {
        return;
      }
      m_connectionState = RESETTED;
      DisplayHeaderCommandImpl headerCommand;
      sendCommand(headerCommand);
      break;
    }
    case RESETTED: {
      if (!expectOk(message)) {
        return;
      }
      m_connectionState = DEV_CONFIG;
      SelectProtocolObdCommandImpl protocolCommand;
      sendCommand(protocolCommand);
      break;
    }
    case DEV_CONFIG: {
      if (!expectOk(message)) {
        return;
      }
      m_connectionState = INWORK;
      sendNextCommand();
      break;
    }
    case INWORK: {
      if (m_pendingCommand == nullptr) {
        qWarning() << "Ignoring unsolicited response" << message;
        return;
      }

      if (ObdParser::hasPayload(message)) {
        processResult(m_pendingCommand, message);
      } else {
        qDebug() << "No usable payload for" << m_pendingCommand->getCmdId()
                 << "in response" << message;
      }

      m_pendingCommand = nullptr;
      if (m_connectionState == INWORK) {
        m_pollTimer.start();
      }
      break;
    }
    default:
      qWarning() << "Message ignored in state" << m_connectionState;
      break;
  }
}

void ObdService::processResult(AbstractCommand *cmd, const QString &message) {
  if (cmd == nullptr) {
    return;
  }

  cmd->setRawData(message);
  const auto calculated = cmd->calculate(message);
  qDebug() << "Command" << cmd->getCmdId()
           << "completed with result:" << calculated;

  m_result.rawValue =
      QStringLiteral("%1 => %2").arg(message, QString::number(calculated));

  const auto id = cmd->getCmdId();
  if (id == CMD_TEMP_COOLANT) {
    m_result.tempCoolant = calculated;
  } else if (id == CMD_TEMP_AIR_AMBIENT) {
    m_result.tempAirAmb = calculated;
  } else if (id == CMD_TEMP_AIR_INTAKE) {
    m_result.tempAirInt = calculated;
  } else if (id == CMD_TEMP_OIL) {
    m_result.tempOil = calculated;
  } else if (id == CMD_TEMP_GB) {
    m_result.tempGB = calculated;
  } else if (id == CMD_TEMP_COMPR) {
    m_result.tempCOMPR = calculated;
  } else if (id == CMD_TEMP_TC) {
    m_result.tempTC = calculated;
  } else if (id == CMD_TEMP_RD) {
    m_result.tempRD = calculated;
  } else if (id == CMD_LVL_OIL) {
    m_result.lvlOil = calculated;
  } else if (id == CMD_LVL_OIL_VOL) {
    m_result.lvlOilVol = calculated;
  } else if (id == CMD_PRESS_FUEL) {
    m_result.pressFuel = calculated;
  } else if (id == CMD_PRESS_BRAKE) {
    m_result.pressBrake = calculated;
  } else if (id == CMD_LVL_FUEL_LITRES) {
    m_result.lvlFuel = calculated;
  }

  emit updateUI(m_result);
}

bool ObdService::sendCommand(const AbstractCommand &command) {
  if (!m_backend->sendCommand(&command)) {
    fail(tr("Failed to send command %1").arg(command.getCmdId()));
    return false;
  }
  return true;
}

bool ObdService::expectOk(const QString &message) {
  const auto upper = message.toUpper();
  // The adapter may report progress before its final response. Keep waiting
  // instead of treating the progress line as success.
  if (upper.contains(QStringLiteral("SEARCHING")) ||
      upper.contains(QStringLiteral("BUS INIT"))) {
    qDebug() << "Adapter is still initializing:" << message;
    return false;
  }
  if (upper.contains(QLatin1Char('?')) || upper.contains(QStringLiteral("ERROR")) ||
      upper.contains(QStringLiteral("UNABLE")) ||
      upper.contains(QStringLiteral("NO DATA"))) {
    fail(tr("Adapter rejected initialization command: %1").arg(message));
    return false;
  }
  return true;
}

void ObdService::sendNextCommand() {
  if (m_connectionState != INWORK || m_pendingCommand != nullptr ||
      m_commands.empty()) {
    return;
  }

  auto *command = m_commands.at(m_currCmd).get();
  m_currCmd = (m_currCmd + 1) % static_cast<int>(m_commands.size());
  m_pendingCommand = command;
  sendCommand(*command);
}

void ObdService::onDisconnected() {
  if (m_connectionState == NC || m_connectionState == ERROR) {
    return;
  }
  fail(tr("Bluetooth connection lost"));
}

void ObdService::onSocketError(const QString &reason) {
  if (m_connectionState == NC) {
    emit serviceError(reason);
    return;
  }
  fail(reason);
}

void ObdService::fail(const QString &reason) {
  if (m_connectionState == ERROR) {
    return;
  }

  qWarning() << "OBD service error:" << reason;
  m_pollTimer.stop();
  m_pendingCommand = nullptr;
  m_connectionState = ERROR;
  m_backend->stopClient();
  emit serviceError(reason);
}
