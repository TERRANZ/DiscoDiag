#include "MainWindow.h"
#include "src/obd/ObdThread.h"
#include "ui_mainwindow.h"

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
  emit start();
}

void MainWindow::on_action_exit_triggered() {}

void MainWindow::on_update_ui(ObdResult &result) {
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
