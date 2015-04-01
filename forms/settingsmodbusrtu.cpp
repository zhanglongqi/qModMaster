#include <QtDebug>
#include "settingsmodbusrtu.h"
#include "ui_settingsmodbusrtu.h"

SettingsModbusRTU::SettingsModbusRTU(QWidget *parent,ModbusCommSettings * settings) :
    QDialog(parent),
    ui(new Ui::SettingsModbusRTU),
    m_settings(settings)
{
    ui->setupUi(this);

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(changesAccepted()));

}

SettingsModbusRTU::~SettingsModbusRTU()
{
    delete ui;
}

void SettingsModbusRTU::showEvent(QShowEvent * event)
{

    //Load Settings
    if (m_settings != NULL) {

        ui->cmbPort->clear();
        ui->cmbRTS->clear();

        //Populate cmbPort-cmbRTS
        #ifdef Q_OS_WIN32
            ui->cmbPort->addItem("COM1:");
            ui->cmbPort->addItem("COM2:");
            ui->cmbPort->addItem("COM3:");
            ui->cmbPort->addItem("COM4:");
            ui->cmbRTS->addItem("Disable");
            ui->cmbRTS->addItem("Enable");
            ui->cmbRTS->addItem("HandShake");
            ui->cmbRTS->addItem("Toggle");
        #else
            ui->cmbPort->addItem("/dev/ttyS0");
            ui->cmbPort->addItem("/dev/ttyS1");
            ui->cmbPort->addItem("/dev/ttyS2");
            ui->cmbPort->addItem("/dev/ttyS3");
            ui->cmbRTS->addItem("None");
            ui->cmbRTS->addItem("Up");
            ui->cmbRTS->addItem("Down");
        #endif

        //Selection of port name
        if ((ui->cmbPort->findText(m_settings->serialPort()) == -1))
            ui->cmbPort->addItem(m_settings->serialPort());
        ui->cmbPort->addItem("Add Port");

        ui->cmbPort->setCurrentIndex(ui->cmbPort->findText(m_settings->serialPort()));
        ui->cmbBaud->setCurrentIndex(ui->cmbBaud->findText(m_settings->baud()));
        ui->cmbDataBits->setCurrentIndex(ui->cmbDataBits->findText(m_settings->dataBits()));
        ui->cmbStopBits->setCurrentIndex(ui->cmbStopBits->findText(m_settings->stopBits()));
        ui->cmbParity->setCurrentIndex(ui->cmbParity->findText(m_settings->parity()));
        ui->cmbRTS->setCurrentIndex(ui->cmbRTS->findText(m_settings->RTS()));
    }


}

void SettingsModbusRTU::changesAccepted()
{

    //Save Settings
    if (m_settings != NULL) {

        m_settings->setSerialPort(ui->cmbPort->currentText());
        m_settings->setBaud(ui->cmbBaud->currentText());
        m_settings->setDataBits(ui->cmbDataBits->currentText());
        m_settings->setStopBits(ui->cmbStopBits->currentText());
        m_settings->setParity(ui->cmbParity->currentText());
        m_settings->setRTS((QString)ui->cmbRTS->currentText());
    }

}

