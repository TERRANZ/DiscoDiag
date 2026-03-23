#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QString>
#include "src/command/commandIds.h"

class AbstractCommand {
public:
  virtual ~AbstractCommand() = default;

  AbstractCommand();

  AbstractCommand(const CommandId cmdId, const QString &cmdName) {
    m_cmdId = cmdId;
    m_cmdName = cmdName;
  }

    AbstractCommand(const QString &cmdVal, const QString &cmdName) {
    m_cmdId = CMD_NO_VALUE;
    m_cmdValue = cmdVal;
    m_cmdName = cmdName;
  }

  virtual int calculate(const QString &value) { return -1; }

  CommandId getCmdId() const { return m_cmdId; }
  QString getCmdName() const { return m_cmdName; }
  QString getRawData() const { return m_rawData; }

  void setCmdName(const QString &value);

  void setCmdId(const QString &value);

  void setRawData(const QString &value);

protected:
  CommandId m_cmdId;
  QString m_cmdValue;
  QString m_cmdName;
  QString m_rawData;
};

#endif // ABSTRACTCOMMAND_H
