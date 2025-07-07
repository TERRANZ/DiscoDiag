#include "MainWindow.h"
#include "src/obd/ObdThread.h"
#include "ui_mainwindow.h"
#include <qobjectdefs.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    emit stop();
    delete ui;
}

void MainWindow::on_action_start_triggered() {
    const auto obdThread = new ObdThread();
    connect(this, SIGNAL(start()), obdThread, SLOT(start()));
    connect(this, SIGNAL(stop()), obdThread, SLOT(stop()));
    connect(obdThread, SIGNAL(updateUI(ObdResult&)), this, SLOT(update_ui(ObdResult&)));
    emit start();
}

void MainWindow::on_action_exit_triggered() {
}

void MainWindow::update_ui(ObdResult &result) {
    ui->teDebug->moveCursor(QTextCursor::End);
    ui->teDebug->insertPlainText(result.rawValue + "\n");
    ui->teDebug->moveCursor(QTextCursor::End);

    ui->numTempCooland->display(result.tempCoolant);
    ui->numTempAirAmb->display(result.tempAirAmb);
    ui->numTempAirInt->display(result.tempAirInt);
    ui->numTempOil->display(result.tempOil);
    ui->numTempGb->display(result.tempGB);

    ui->dTempCoolant->setValue(result.tempCoolant);
    ui->dTempAirAmb->setValue(result.tempAirAmb);
    ui->dTempAirInt->setValue(result.tempAirInt);
    ui->dTempOil->setValue(result.tempOil);
    ui->dTempGb->setValue(result.tempGB);
}

void MainWindow::on_action_settings_triggered() {
    const auto localAdapters = QBluetoothLocalDevice::allDevices();
    const QBluetoothAddress adapter = localAdapters.isEmpty() ? QBluetoothAddress() : localAdapters.at(0).address();
    auto rs = RemoteSelector(adapter, this);
    rs.startDiscovery();
}