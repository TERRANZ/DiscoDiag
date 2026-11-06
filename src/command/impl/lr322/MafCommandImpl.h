//
// Created by terranz on 06.11.26.
//

#ifndef MAFCOMMANDIMPL_H
#define MAFCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class MafCommandImpl final : public AbstractCommand {
public:
    MafCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // MAFCOMMANDIMPL_H
