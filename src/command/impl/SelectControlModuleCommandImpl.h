#ifndef DISCODIAG_SELECTCONTROLMODULECOMMANDIMPL_H
#define DISCODIAG_SELECTCONTROLMODULECOMMANDIMPL_H

#include "../abstractcommand.h"

class SelectControlModuleCommandImpl : public AbstractCommand {
public:
    SelectControlModuleCommandImpl(const QString &cmdId, const QString &cmdName);
};


#endif //DISCODIAG_SELECTCONTROLMODULECOMMANDIMPL_H
