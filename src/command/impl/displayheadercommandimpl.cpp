#include "displayheadercommandimpl.h"

DisplayHeaderCommandImpl::DisplayHeaderCommandImpl() : AbstractCommand("ATH1", "Set display headers parameter.") {

}

QString DisplayHeaderCommandImpl::calculate(const QString &value) {
    return AbstractCommand::calculate(value);
}
