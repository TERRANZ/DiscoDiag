#include "abstractcommand.h"

AbstractCommand::AbstractCommand()
{

}

QString AbstractCommand::getCmdId() const
{
    return cmdId;
}

void AbstractCommand::setCmdId(const QString &value)
{
    cmdId = value;
}

QString AbstractCommand::getCmdName() const
{
    return cmdName;
}

void AbstractCommand::setCmdName(const QString &value)
{
    cmdName = value;
}


QString AbstractCommand::getRawData() const
{
    return rawData;
}

void AbstractCommand::setRawData(const QString &value)
{
    rawData = value;
}
