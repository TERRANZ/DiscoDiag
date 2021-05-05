#ifndef DISCODIAG_TRANSFERCASEROTENGCOMMANDIMPL_H
#define DISCODIAG_TRANSFERCASEROTENGCOMMANDIMPL_H

#include "src/command/abstractcommand.h"

class TransferCaseRotEngCommandImpl : public AbstractCommand {
public:
    TransferCaseRotEngCommandImpl();

    QString calculate(const QString &value) const override;
};


#endif //DISCODIAG_TRANSFERCASEROTENGCOMMANDIMPL_H
