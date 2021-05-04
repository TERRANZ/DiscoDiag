#ifndef DISCODIAG_SUSPENSIONHEIGHTCOMMANDIMPL_H
#define DISCODIAG_SUSPENSIONHEIGHTCOMMANDIMPL_H

#include "src/command/abstractcommand.h"

class SuspensionHeightCommandImpl : AbstractCommand {
public:
    SuspensionHeightCommandImpl(QString &wheel);
};


#endif //DISCODIAG_SUSPENSIONHEIGHTCOMMANDIMPL_H
