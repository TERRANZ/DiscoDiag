#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    service = new ObdService(this);
    connect(service, SIGNAL(updateUI(ObdResult&)), this, SLOT(on_update_ui(ObdResult&)));
}

MainWindow::~MainWindow() {
    service->stopService();
    delete ui;
}

void MainWindow::on_action_start_triggered() {
    // auto *thread = new QThread;
    // service->moveToThread(thread);
    service->startService();
}

void MainWindow::on_action_exit_triggered() {
}

void MainWindow::on_update_ui(ObdResult &result) {
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText(result.rawValue + "\n");
    ui->textEdit->moveCursor(QTextCursor::End);
}
