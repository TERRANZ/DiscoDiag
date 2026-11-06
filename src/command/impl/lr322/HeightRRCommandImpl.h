//
// Created by terranz on 06.11.26.
//

#ifndef HEIGHTRRCOMMANDIMPL_H
#define HEIGHTRRCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class HeightRRCommandImpl final : public AbstractCommand {
public:
    HeightRRCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // HEIGHTRRCOMMANDIMPL_H
