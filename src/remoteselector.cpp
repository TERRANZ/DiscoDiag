#include "remoteselector.h"
#include "ui_remoteselector.h"

RemoteSelector::RemoteSelector(const QBluetoothAddress &localAdapter, QWidget *parent)
    : QDialog(parent), ui(new Ui::RemoteSelector)
{
    ui->setupUi(this);

    m_discoveryAgent = new QBluetoothServiceDiscoveryAgent(localAdapter);

    connect(m_discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
            this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));
    connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
    connect(m_discoveryAgent, SIGNAL(canceled()), this, SLOT(discoveryFinished()));
}

RemoteSelector::~RemoteSelector()
{
    delete ui;
    delete m_discoveryAgent;
}

void RemoteSelector::startDiscovery()
{
    ui->status->setText(tr("Scanning..."));
    if (m_discoveryAgent->isActive())
        m_discoveryAgent->stop();

    ui->remoteDevices->clear();

    m_discoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
}

void RemoteSelector::stopDiscovery()
{
    if (m_discoveryAgent)
    {
        m_discoveryAgent->stop();
    }
}

QBluetoothServiceInfo RemoteSelector::service() const
{
    return m_service;
}

void RemoteSelector::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
    qDebug() << "Discovered service on"
             << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();
    qDebug() << "\tDescription:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
    qDebug() << "\tProvider:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
    qDebug() << "\tL2CAP protocol service multiplexer:"
             << serviceInfo.protocolServiceMultiplexer();
    qDebug() << "\tRFCOMM server channel:" << serviceInfo.serverChannel();
    qDebug() << "\tService uuid: " << serviceInfo.serviceUuid();

    QString remoteName;
    QString serviceName = serviceInfo.serviceName();

    remoteName = serviceInfo.device().name();

    qDebug() << "Adding to list remote name " << remoteName << " with serviceName " << serviceName;

    QListWidgetItem *item = new QListWidgetItem(QString::fromLatin1("%1 %2").arg(remoteName, serviceName));

    m_discoveredServices.insert(item, serviceInfo);
    ui->remoteDevices->addItem(item);
}

void RemoteSelector::discoveryFinished()
{
    ui->status->setText(tr("Select the chat service to connect to."));
}

void RemoteSelector::on_remoteDevices_itemActivated(QListWidgetItem *item)
{
    qDebug() << "got click" << item->text();
    m_service = m_discoveredServices.value(item);
    if (m_discoveryAgent->isActive())
        m_discoveryAgent->stop();

    accept();
}

void RemoteSelector::on_cancelButton_clicked()
{
    reject();
}
