//
// Created by terranz on 06.11.26.
//

#ifndef ACCELERCOMMANDIMPL_H
#define ACCELERCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class AccelerCommandImpl final : public AbstractCommand {
public:
    AccelerCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // ACCELERCOMMANDIMPL_H
