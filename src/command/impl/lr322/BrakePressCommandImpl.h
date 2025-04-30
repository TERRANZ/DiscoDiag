//
// Created by terranz on 27.02.25.
//

#ifndef BRAKEPRESSCOMMANDIMPL_H
#define BRAKEPRESSCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"

class BrakePressCommandImpl final : public AbstractCommand
{
public:
  BrakePressCommandImpl();
  int calculate(const QString &value) override;
};

#endif // BRAKEPRESSCOMMANDIMPL_H
