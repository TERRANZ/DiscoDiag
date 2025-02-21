#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  service = new ObdService(this);
}

MainWindow::~MainWindow() {
  service->stopService();
  delete ui;
}

void MainWindow::on_action_start_triggered() {
  auto *thread = new QThread;
  service->moveToThread(thread);
  service->startService();
}

void MainWindow::on_action_exit_triggered() {
}