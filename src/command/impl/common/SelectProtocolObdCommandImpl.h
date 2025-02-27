#ifndef SELECTPROTOCOLOBDCOMMANDIMPL_H
#define SELECTPROTOCOLOBDCOMMANDIMPL_H

#include "src/command/AbstractCommand.h"

class SelectProtocolObdCommandImpl : public AbstractCommand {
public:
    SelectProtocolObdCommandImpl();

    QString calculate(const QString &value) override;
};

#endif //SELECTPROTOCOLOBDCOMMANDIMPL_H
