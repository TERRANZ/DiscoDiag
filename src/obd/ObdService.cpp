#include "ObdService.h"
#include <qdebug.h>
#include <qlogging.h>
#include <qthread.h>
#include <src/command/commandIds.h>

#include "src/command/impl/common/DisplayHeaderCommandImpl.h"
#include "src/command/impl/common/ObdResetFixCommandImpl.h"
#include "src/command/impl/common/SelectProtocolObdCommandImpl.h"
#include "src/command/impl/lr322/AmbientAirTempCommandImpl.h"
#include "src/command/impl/lr322/CoolantTempCommandImpl.h"
#include "src/command/impl/lr322/GbTempCommandImpl.h"
#include "src/command/impl/lr322/IntakeAirTempCommandImpl.h"
#include "src/command/impl/lr322/OilTempCommandImpl.h"
#include "src/command/impl/lr322/ComprTempCommandImpl.h"
#include "src/obd/ObdParser.h"
#include "src/ui/MainWindow.h"

ObdService::ObdService(QObject *parent) : QObject(parent)
{
  m_backend = new BtBackend(this);
  m_commands.insert(CMD_TEMP_COOLANT, new CoolantTempCommandImpl());
  m_commands.insert(CMD_TEMP_AIR_AMBIENT, new AmbientAirTempCommandImpl());
  m_commands.insert(CMD_TEMP_AIR_INTAKE, new IntakeAirTempCommandImpl());
  m_commands.insert(CMD_TEMP_OIL, new OilTempCommandImpl());
  m_commands.insert(CMD_TEMP_GB, new GbTempCommandImpl());
  m_commands.insert(CMD_TEMP_COMPR, new ComprTempCommandImpl());
}

void ObdService::startService()
{
  if (QList<QBluetoothAddress> adapters = BtBackend::listAdapters();
      adapters.empty())
  {
    qDebug() << "No adapters found";
    emit serviceError("No adapters found");
  }
  else
  {
    m_backend = new BtBackend(this);
    connect(m_backend, &BtBackend::messageReceived, this,
            &ObdService::messageReceived);
    connect(m_backend, QOverload<const QString &>::of(&BtBackend::connected),
            this, &ObdService::connected);
    m_connectionState = DEV_SELECTED;
    m_backend->startClient();
  }
}

void ObdService::stopService() { m_backend->stopClient(); }

void ObdService::messageReceived(const QString &message)
{
  processMessage(message);

  doObdLoop();
}

void ObdService::processMessage(const QString &message)
{
  switch (m_connectionState)
  {
  case CONNECTED:
    qDebug() << "Connection state CONNECTED => RESETTED";
    m_connectionState = RESETTED;
    break;
  case RESETTED:
    qDebug() << "Connection state RESETTED => DEV_CONFIG";
    m_connectionState = DEV_CONFIG;
    break;
  case DEV_CONFIG:
    qDebug() << "Connection state DEV_CONFIG => INWORK";
    m_connectionState = INWORK;
    break;
  case INWORK:
  {
    qDebug() << "Received message " << message;
    const auto cmd = m_commands.value(ObdParser::extractCommandId(message));
    processResult(cmd, message);
    QThread::msleep(100);
  }
  break;
  case ERROR:
    break;
  }
}

void ObdService::connected(const QString &name)
{
  qDebug() << "Connected to:" << name;
  m_connectionState = CONNECTED;

  doObdLoop();
}

void ObdService::processResult(AbstractCommand *cmd, const QString &message)
{
  if (cmd == nullptr)
  {
    qDebug() << "Command not found!";
    return;
  }
  const auto calculated = cmd->calculate(message);
  qDebug() << "Command " << cmd->getCmdId()
           << " completed with result: " << calculated;
  m_result.rawValue = cmd->getCmdId() + " = " + QString::number(calculated);
  if (cmd->getCmdId() == CMD_TEMP_COOLANT)
  {
    m_result.tempCoolant = calculated;
  }
  if (cmd->getCmdId() == CMD_TEMP_AIR_AMBIENT)
  {
    m_result.tempAirAmb = calculated;
  }
  if (cmd->getCmdId() == CMD_TEMP_AIR_INTAKE)
  {
    m_result.tempAirInt = calculated;
  }
  if (cmd->getCmdId() == CMD_TEMP_OIL)
  {
    m_result.tempOil = calculated;
  }
  if (cmd->getCmdId() == CMD_TEMP_GB)
  {
    m_result.tempGB = calculated;
  }
  emit updateUI(m_result);
}

void ObdService::doObdLoop()
{
  doObdPreparationStep();

  if (m_connectionState == INWORK)
  {
    m_backend->sendCommand(m_commands.values().at(m_curr_cmd));
    m_curr_cmd++;
    if (m_curr_cmd >= m_commands.size())
    {
      m_curr_cmd = 0;
    }
  }
  if (m_connectionState == ERROR)
  {
  }
}

void ObdService::doObdPreparationStep()
{
  switch (m_connectionState)
  {
  case CONNECTED:
    m_backend->sendCommand(new ObdResetFixCommandImpl());
    break;
  case RESETTED:
    m_backend->sendCommand(new DisplayHeaderCommandImpl());
    break;
  case DEV_CONFIG:
    m_backend->sendCommand(new SelectProtocolObdCommandImpl());
    break;
  }
}
