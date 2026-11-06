//
// Created by terranz on 06.11.26.
//

#ifndef ABSRLCOMMANDIMPL_H
#define ABSRLCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class AbsRLCommandImpl final : public AbstractCommand {
public:
    AbsRLCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // ABSRLCOMMANDIMPL_H
