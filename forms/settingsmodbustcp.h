#ifndef SETTINGSMODBUSTCP_H
#define SETTINGSMODBUSTCP_H

#include <QDialog>
#include <QSettings>
#include <QAbstractButton>

#include "src/modbuscommsettings.h"

namespace Ui {
    class SettingsModbusTCP;
}

class SettingsModbusTCP : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsModbusTCP(QWidget *parent = 0 ,ModbusCommSettings * settings = 0);
    ~SettingsModbusTCP();

private:
    Ui::SettingsModbusTCP *ui;
    ModbusCommSettings * m_settings;
    int validateInputs();

private slots:
    void changesAccepted();


protected:
    void showEvent(QShowEvent * event);

};

#endif // SETTINGSMODBUSTCP_H
