//
// Created by terranz on 27.02.25.
//

#ifndef COMPRTEMPCOMMANDIMPL_H
#define COMPRTEMPCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"

class ComprTempCommandImpl final : public AbstractCommand
{
public:
    ComprTempCommandImpl();
    int calculate(const QString &value) override;
};

#endif // COMPRTEMPCOMMANDIMPL_H
