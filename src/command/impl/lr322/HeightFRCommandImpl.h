//
// Created by terranz on 06.11.26.
//

#ifndef HEIGHTFRCOMMANDIMPL_H
#define HEIGHTFRCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class HeightFRCommandImpl final : public AbstractCommand {
public:
    HeightFRCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // HEIGHTFRCOMMANDIMPL_H
