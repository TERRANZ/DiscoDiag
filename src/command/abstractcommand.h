#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QString>

class AbstractCommand
{
public:
    AbstractCommand();
    virtual QString calculate() { return QString(""); }
    QString GetRawData()
    {
        return rawData;
    }
    QString GetId()
    {
        return cmdId;
    }
    QString GetName()
    {
        return cmdName;
    }

protected:
    QString cmdId;
    QString cmdName;
    QString rawData;
};

#endif // ABSTRACTCOMMAND_H
