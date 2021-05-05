#ifndef DISCODIAG_TRANSFERCASESOLENOIDPOSITIONCOMMANDIMPL_H
#define DISCODIAG_TRANSFERCASESOLENOIDPOSITIONCOMMANDIMPL_H

#include "src/command/abstractcommand.h"

class TransferCaseSolenoidPositionCommandImpl : public AbstractCommand {
public:
    TransferCaseSolenoidPositionCommandImpl();

    QString calculate(const QString &value) const override;
};


#endif //DISCODIAG_TRANSFERCASESOLENOIDPOSITIONCOMMANDIMPL_H
