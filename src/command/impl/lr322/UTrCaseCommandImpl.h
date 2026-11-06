//
// Created by terranz on 06.11.26.
//

#ifndef UTRCASECOMMANDIMPL_H
#define UTRCASECOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class UTrCaseCommandImpl final : public AbstractCommand {
public:
    UTrCaseCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // UTRCASECOMMANDIMPL_H
