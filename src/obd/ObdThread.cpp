//
// Created by terranz on 03.03.25.
//

#include "ObdThread.h"

ObdThread::ObdThread() {
    service = new ObdService();
    connect(service, SIGNAL(updateUI(ObdResult&)), this, SLOT(on_update_ui(ObdResult&)));
}

void ObdThread::start() {
    service->startService();
}

void ObdThread::stop() {
    service->stopService();
}


void ObdThread::on_update_ui(ObdResult &result) {
    emit this->updateUI(result);
}
