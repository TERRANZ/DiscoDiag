//
// Created by terranz on 06.11.26.
//

#ifndef MAINPRESSCOMMANDIMPL_H
#define MAINPRESSCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class MainPressCommandImpl final : public AbstractCommand {
public:
    MainPressCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // MAINPRESSCOMMANDIMPL_H
