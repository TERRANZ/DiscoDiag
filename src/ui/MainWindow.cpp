#include "MainWindow.h"

#include "src/obd/ObdService.h"
#include "src/ui/RemoteSelector.h"
#include "ui_mainwindow.h"

#include <QBluetoothLocalDevice>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QSettings>
#include <QStatusBar>

namespace {
const QString DEFAULT_DEVICE_ADDRESS = QStringLiteral("10:21:3E:4A:3E:41");
const QString DEVICE_ADDRESS_KEY = QStringLiteral("bluetooth/address");
constexpr int DEBUG_HISTORY_LINES = 500;
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(std::make_unique<Ui::MainWindow>()) {
  ui->setupUi(this);
  ui->teDebug->setMaximumBlockCount(DEBUG_HISTORY_LINES);

  QSettings settings;
  m_deviceAddress = QBluetoothAddress(
      settings.value(DEVICE_ADDRESS_KEY, DEFAULT_DEVICE_ADDRESS).toString());

  m_service = new ObdService(this);
  connect(m_service, &ObdService::updateUI, this, &MainWindow::update_ui);
  connect(m_service, &ObdService::serviceError, this,
          &MainWindow::onServiceError);

  statusBar()->showMessage(tr("Готово"));
}

MainWindow::~MainWindow() {
  m_service->stopService();
}

void MainWindow::on_action_start_triggered() {
  if (m_service->isRunning()) {
    statusBar()->showMessage(tr("Диагностика уже запущена"));
    return;
  }

  if (!m_service->startService(m_deviceAddress)) {
    statusBar()->showMessage(tr("Не удалось запустить диагностику"));
    return;
  }

  appendDebug(tr("Запуск диагностики: %1").arg(m_deviceAddress.toString()));
  statusBar()->showMessage(
      tr("Подключение к %1...").arg(m_deviceAddress.toString()));
}

void MainWindow::on_action_stop_triggered() {
  m_service->stopService();
  appendDebug(tr("Диагностика остановлена"));
  statusBar()->showMessage(tr("Диагностика остановлена"));
}

void MainWindow::on_action_exit_triggered() {
  m_service->stopService();
  close();
}

void MainWindow::on_action_triggered() {
  QMessageBox::about(
      this, tr("О программе"),
      tr("DiscoDiag — диагностика Land Rover Range Rover L322 "
         "через OBD2/Bluetooth."));
}

void MainWindow::on_action_settings_triggered() {
  const auto adapters = QBluetoothLocalDevice::allDevices();
  if (adapters.isEmpty()) {
    QMessageBox::warning(this, tr("Настройка Bluetooth"),
                         tr("Локальный Bluetooth-адаптер не найден."));
    return;
  }

  RemoteSelector selector(adapters.at(0).address(), this);
  selector.startDiscovery();
  if (selector.exec() != QDialog::Accepted) {
    return;
  }

  const auto selectedService = selector.service();
  const auto address = selectedService.device().address();
  if (address.isNull()) {
    QMessageBox::warning(this, tr("Настройка Bluetooth"),
                         tr("Не удалось определить адрес выбранного устройства."));
    return;
  }

  m_deviceAddress = address;
  QSettings settings;
  settings.setValue(DEVICE_ADDRESS_KEY, address.toString());

  appendDebug(tr("Выбрано устройство %1 (%2)")
                  .arg(selectedService.device().name(), address.toString()));
  statusBar()->showMessage(
      tr("Устройство выбрано: %1").arg(address.toString()));
}

void MainWindow::update_ui(const ObdResult &result) {
  ui->numTempCoolant->display(result.tempCoolant);
  ui->numTempAirAmb->display(result.tempAirAmb);
  ui->numTempAirInt->display(result.tempAirInt);
  ui->numTempOil->display(result.tempOil);
  ui->numTempGb->display(result.tempGB);
  ui->numTempCompr->display(result.tempCOMPR);
  ui->numTempTc->display(result.tempTC);
  ui->numTempRd->display(result.tempRD);
  ui->numVolOil->display(result.lvlOilVol);

  ui->dTempCoolant->setValue(result.tempCoolant);
  ui->dTempAirAmb->setValue(result.tempAirAmb);
  ui->dTempAirInt->setValue(result.tempAirInt);
  ui->dTempOil->setValue(result.tempOil);
  ui->dTempGb->setValue(result.tempGB);
  ui->dTempCompr->setValue(result.tempCOMPR);
  ui->dTempTc->setValue(result.tempTC);
  ui->dTempRd->setValue(result.tempRD);
  ui->dlvlOil->setValue(result.lvlOil);

  if (!result.rawValue.isEmpty()) {
    appendDebug(result.rawValue);
  }
}

void MainWindow::onServiceError(const QString &reason) {
  appendDebug(tr("Ошибка: %1").arg(reason));
  statusBar()->showMessage(reason, 10000);
  QMessageBox::warning(this, tr("Ошибка диагностики"), reason);
}

void MainWindow::appendDebug(const QString &text) {
  ui->teDebug->appendPlainText(text);
}
