#include "SelectControlModuleCommandImpl.h"

SelectControlModuleCommandImpl::SelectControlModuleCommandImpl(const QString &cmdId, const QString &cmdName)
        : AbstractCommand("ATSH" + cmdId, cmdName) {
}