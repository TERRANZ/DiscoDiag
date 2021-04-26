#ifndef REMOTESELECTOR_H
#define REMOTESELECTOR_H

#include <QDialog>
#include <QBluetoothAddress>
#include <QBluetoothUuid>
#include <QBluetoothServiceInfo>
#include <QBluetoothServiceDiscoveryAgent>
#include <QMap>
#include <QListWidgetItem>

namespace Ui {
class RemoteSelector;
}

class RemoteSelector : public QDialog
{
    Q_OBJECT

public:
    explicit RemoteSelector(const QBluetoothAddress &localAdapter, QWidget *parent = nullptr);
    ~RemoteSelector();

    void startDiscovery();
    void stopDiscovery();
    QBluetoothServiceInfo service() const;

private:
    Ui::RemoteSelector *ui;

    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;
    QBluetoothServiceInfo m_service;
    QMap<QListWidgetItem *, QBluetoothServiceInfo> m_discoveredServices;

private slots:
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void discoveryFinished();
    void on_remoteDevices_itemActivated(QListWidgetItem *item);
    void on_cancelButton_clicked();
};

#endif // REMOTESELECTOR_H
