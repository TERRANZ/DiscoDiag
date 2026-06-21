//
// Created by terranz on 27.02.25.
//

#ifndef OILVOLUMECOMMANDIMPL_H
#define OILVOLUMECOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class OilVolumeCommandImpl final : public AbstractCommand {
public:
    OilVolumeCommandImpl();

    int calculate(const QString &value) override;
};


#endif //OILVOLUMECOMMANDIMPL_H
