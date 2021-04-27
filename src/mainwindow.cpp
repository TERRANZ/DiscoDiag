#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    service = new ObdService(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_action_3_triggered() {
    service->startService();
}
