#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QString>
#include <utility>

#include "src/obd/ObdResult.h"

class AbstractCommand {
public:
  AbstractCommand() = default;

  AbstractCommand(QString cmdId, QString cmdName,
                  int ObdResult::*resultField = nullptr)
      : m_cmdId(std::move(cmdId)),
        m_cmdName(std::move(cmdName)),
        m_resultField(resultField) {}

  virtual ~AbstractCommand() = default;

  AbstractCommand(const AbstractCommand &) = delete;
  AbstractCommand &operator=(const AbstractCommand &) = delete;

  virtual int calculate(const QString &) const { return -1; }

  QString getCmdId() const { return m_cmdId; }
  QString getCmdName() const { return m_cmdName; }
  QString getRawData() const { return m_rawData; }

  // Field of ObdResult that receives calculate()'s value, or nullptr for
  // commands that only affect the adapter configuration.
  int ObdResult::*getResultField() const { return m_resultField; }

  void setCmdName(const QString &value) { m_cmdName = value; }

  void setCmdId(const QString &value) { m_cmdId = value; }

  void setRawData(const QString &value) { m_rawData = value; }

protected:
  QString m_cmdId;
  QString m_cmdName;
  QString m_rawData;
  int ObdResult::*m_resultField = nullptr;
};

#endif // ABSTRACTCOMMAND_H
