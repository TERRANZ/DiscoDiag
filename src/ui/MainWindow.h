#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/obd/ObdService.h"
#include "src/obd/ObdResult.h"
QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_action_start_triggered();

    void on_action_exit_triggered();

    void on_update_ui(ObdResult &result);

private:
    Ui::MainWindow *ui;
    ObdService *service;
};

#endif // MAINWINDOW_H
