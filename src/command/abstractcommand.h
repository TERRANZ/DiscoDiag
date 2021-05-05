#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QString>

class AbstractCommand {
public:
    AbstractCommand();
    AbstractCommand(const QString &cmdId, const QString &cmdName);

    virtual QString calculate(const QString &value) const { return QString(""); }

    QString getCmdId() const;

    void setCmdId(const QString &value);

    QString getCmdName() const;

    void setCmdName(const QString &value);

    QString getRawData() const;

    void setRawData(const QString &value);

protected:
    QString cmdId;
    QString cmdName;
    QString rawData;
};

#endif // ABSTRACTCOMMAND_H
