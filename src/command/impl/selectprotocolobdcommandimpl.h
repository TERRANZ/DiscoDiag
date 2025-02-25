#ifndef SELECTPROTOCOLOBDCOMMANDIMPL_H
#define SELECTPROTOCOLOBDCOMMANDIMPL_H

#include "../abstractcommand.h"

class SelectProtocolObdCommandImpl : public AbstractCommand {
public:
    SelectProtocolObdCommandImpl();

    QString calculate(const QString &value) override;
};

#endif //SELECTPROTOCOLOBDCOMMANDIMPL_H
