//
// Created by terranz on 03.03.25.
//

#ifndef OBDTHREAD_H
#define OBDTHREAD_H
#include <QThread>

#include "ObdResult.h"
#include "ObdService.h"

class ObdService;

class ObdThread : public QThread {
    Q_OBJECT

public:
    ObdThread();

signals:
    void updateUI(ObdResult &result);

public slots:
    void start();

    void stop();

    void on_update_ui(ObdResult &result);

private:
    ObdService *service;
};


#endif //OBDTHREAD_H
