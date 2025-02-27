#ifndef OBDRESETFIXCOMMANDIMPL_H
#define OBDRESETFIXCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"

class ObdResetFixCommandImpl : public AbstractCommand {
public:
    ObdResetFixCommandImpl();

    QString calculate(const QString &value) override;
};

#endif // OBDRESETFIXCOMMANDIMPL_H
