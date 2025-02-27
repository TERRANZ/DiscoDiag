//
// Created by terranz on 27.02.25.
//

#ifndef OILTEMPCOMMANDIMPL_H
#define OILTEMPCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class OilTempCommandImpl final : public AbstractCommand {
public:
    OilTempCommandImpl();

    QString calculate(const QString &value) override;
};


#endif //OILTEMPCOMMANDIMPL_H
