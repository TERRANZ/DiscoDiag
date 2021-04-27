#include "abstractcommand.h"

AbstractCommand::AbstractCommand(const QString &cmdId, const QString &cmdName) {
    this->cmdId = cmdId;
    this->cmdName = cmdName;
}

QString AbstractCommand::getCmdId() const {
    return cmdId;
}

void AbstractCommand::setCmdId(const QString &value) {
    cmdId = value;
}

QString AbstractCommand::getCmdName() const {
    return cmdName;
}

void AbstractCommand::setCmdName(const QString &value) {
    cmdName = value;
}


QString AbstractCommand::getRawData() const {
    return rawData;
}

void AbstractCommand::setRawData(const QString &value) {
    rawData = value;
}
