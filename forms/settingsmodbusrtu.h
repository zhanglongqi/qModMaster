#ifndef SETTINGSMODBUSRTU_H
#define SETTINGSMODBUSRTU_H

#include <QDialog>
#include <QSettings>

#include "src/modbuscommsettings.h"

namespace Ui {
    class SettingsModbusRTU;
}

class SettingsModbusRTU : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsModbusRTU(QWidget *parent = 0 ,ModbusCommSettings * settings = 0);
    ~SettingsModbusRTU();

private:
    Ui::SettingsModbusRTU *ui;
    ModbusCommSettings * m_settings;

private slots:
    void changesAccepted();

protected:
    void showEvent(QShowEvent * event);

};

#endif // SETTINGSMODBUSRTU_H
