#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/obd/ObdResult.h"
#include <QMainWindow>
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

signals:
  void start();
  void stop();

private slots:
  void on_action_start_triggered();

  void on_action_exit_triggered();

  void update_ui(ObdResult &result);

  void on_action_settings_triggered();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
