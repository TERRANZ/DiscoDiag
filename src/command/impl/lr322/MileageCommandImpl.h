//
// Created by terranz on 06.11.26.
//

#ifndef MILEAGECOMMANDIMPL_H
#define MILEAGECOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class MileageCommandImpl final : public AbstractCommand {
public:
    MileageCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // MILEAGECOMMANDIMPL_H
