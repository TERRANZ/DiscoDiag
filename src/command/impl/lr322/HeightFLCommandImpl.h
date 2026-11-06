//
// Created by terranz on 06.11.26.
//

#ifndef HEIGHTFLCOMMANDIMPL_H
#define HEIGHTFLCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class HeightFLCommandImpl final : public AbstractCommand {
public:
    HeightFLCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // HEIGHTFLCOMMANDIMPL_H
