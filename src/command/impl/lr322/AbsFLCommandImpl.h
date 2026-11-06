//
// Created by terranz on 06.11.26.
//

#ifndef ABSFLCOMMANDIMPL_H
#define ABSFLCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class AbsFLCommandImpl final : public AbstractCommand {
public:
    AbsFLCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // ABSFLCOMMANDIMPL_H
