#ifndef DISCODIAG_TRANSFERCASETEMPCOMMANDIMPL_H
#define DISCODIAG_TRANSFERCASETEMPCOMMANDIMPL_H

#include "src/command/abstractcommand.h"

class TransferCaseTempCommandImpl : public AbstractCommand {
public:
    TransferCaseTempCommandImpl();

    QString calculate(const QString &value) const override;
};


#endif //DISCODIAG_TRANSFERCASETEMPCOMMANDIMPL_H
