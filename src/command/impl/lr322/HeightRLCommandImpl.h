//
// Created by terranz on 06.11.26.
//

#ifndef HEIGHTRLCOMMANDIMPL_H
#define HEIGHTRLCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class HeightRLCommandImpl final : public AbstractCommand {
public:
    HeightRLCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // HEIGHTRLCOMMANDIMPL_H
