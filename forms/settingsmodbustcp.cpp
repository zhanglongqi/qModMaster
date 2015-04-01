#include <QtDebug>
#include <QMessageBox>
#include "settingsmodbustcp.h"
#include "ui_settingsmodbustcp.h"

SettingsModbusTCP::SettingsModbusTCP(QWidget *parent, ModbusCommSettings * settings) :
    QDialog(parent),
    ui(new Ui::SettingsModbusTCP),
    m_settings(settings)
{
    ui->setupUi(this);

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(changesAccepted()));
}

SettingsModbusTCP::~SettingsModbusTCP()
{
    delete ui;
}

void SettingsModbusTCP::showEvent(QShowEvent * event)
{

    //Load Settings
    if (m_settings != NULL) {
        ui->leTCPPort->setText(m_settings->TCPPort());
        ui->leSlaveIP->setText(m_settings->slaveIP());
    }

}

void SettingsModbusTCP::changesAccepted()
{
    int validation;

    validation = validateInputs();
    switch(validation){
        case 0 : // ok
            //Save Settings
            if (m_settings != NULL) {
                m_settings->setTCPPort(ui->leTCPPort->text());
                m_settings->setSlaveIP(ui->leSlaveIP->text());
            }
            break;
        case 1 : // wrong ip
            QMessageBox::critical(NULL, "Modbus TCP Settings","Wrong IP Address.");
            break;
        case 2 : // wrong port
            QMessageBox::critical(NULL, "Modbus TCP Settings","Wrong Port Number.");
            break;
    }

}

int SettingsModbusTCP::validateInputs()
{
    //Strip zero's from IP
    QStringList ipBytes;
    bool ok;
    int i, ipByte, port;

    ipBytes = (ui->leSlaveIP->text()).split(".");
    if (ipBytes.size() == 4){
        for (i = 0; i < ipBytes.size(); i++){
            ipByte = ipBytes[i].toInt(&ok);
            if (!ok || ipByte > 255 )
                return 1; // wrong ip
        }
    }
    else
        return 1; // wrong ip

    port = (ui->leTCPPort->text()).toInt(&ok);
    if (!ok || port <= 0 || port > 65535)
        return 2; // wrong port

    return 0; // validate ok

}
