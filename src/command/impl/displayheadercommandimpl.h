#ifndef DISCODIAG_DISPLAYHEADERCOMMANDIMPL_H
#define DISCODIAG_DISPLAYHEADERCOMMANDIMPL_H

#include "../abstractcommand.h"

class DisplayHeaderCommandImpl : public AbstractCommand {
public:
    DisplayHeaderCommandImpl();

    QString calculate(const QString &value) override;
};


#endif //DISCODIAG_DISPLAYHEADERCOMMANDIMPL_H
