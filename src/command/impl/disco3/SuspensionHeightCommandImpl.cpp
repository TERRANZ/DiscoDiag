#include <src/command/commandIds.h>
#include "SuspensionHeightCommandImpl.h"

SuspensionHeightCommandImpl::SuspensionHeightCommandImpl(QString &wheel) : AbstractCommand(SUSP_HEIGHT + wheel,
                                                                                           "Suspension height") {

}
