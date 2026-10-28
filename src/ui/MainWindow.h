#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/obd/ObdResult.h"

#include <QBluetoothAddress>
#include <QMainWindow>

#include <memory>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class ObdService;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

  ~MainWindow() override;

private slots:
  void on_action_start_triggered();

  void on_action_stop_triggered();

  void on_action_exit_triggered();

  void on_action_triggered();

  void on_action_settings_triggered();

  void update_ui(const ObdResult &result);

  void onServiceError(const QString &reason);

private:
  void appendDebug(const QString &text);

  std::unique_ptr<Ui::MainWindow> ui;
  ObdService *m_service = nullptr;
  QBluetoothAddress m_deviceAddress;
};

#endif // MAINWINDOW_H
