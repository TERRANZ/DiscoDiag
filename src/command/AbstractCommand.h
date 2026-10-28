#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QString>
#include <utility>

class AbstractCommand {
public:
  AbstractCommand() = default;

  AbstractCommand(QString cmdId, QString cmdName)
      : m_cmdId(std::move(cmdId)), m_cmdName(std::move(cmdName)) {}

  virtual ~AbstractCommand() = default;

  AbstractCommand(const AbstractCommand &) = delete;
  AbstractCommand &operator=(const AbstractCommand &) = delete;

  virtual int calculate(const QString &) const { return -1; }

  QString getCmdId() const { return m_cmdId; }
  QString getCmdName() const { return m_cmdName; }
  QString getRawData() const { return m_rawData; }

  void setCmdName(const QString &value) { m_cmdName = value; }

  void setCmdId(const QString &value) { m_cmdId = value; }

  void setRawData(const QString &value) { m_rawData = value; }

protected:
  QString m_cmdId;
  QString m_cmdName;
  QString m_rawData;
};

#endif // ABSTRACTCOMMAND_H
