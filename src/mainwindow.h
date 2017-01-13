#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QSettings>
#include <QtWidgets/QLabel>

#include "forms/about.h"
#include "forms/settingsmodbusrtu.h"
#include "forms/settingsmodbustcp.h"
#include "forms/settings.h"
#include "forms/busmonitor.h"
#include "modbuscommsettings.h"
#include "modbusadapter.h"

namespace Ui {
    class MainWindow;
}

enum NumBase{
    DEC=0x0,
    HEX=0x1,
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, ModbusAdapter *adapter = 0, ModbusCommSettings *settings = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //UI - Dialogs
    About *m_dlgAbout;
    SettingsModbusRTU *m_dlgModbusRTU;
    SettingsModbusTCP *m_dlgModbusTCP;
    Settings *m_dlgSettings;
    BusMonitor *m_busMonitor;

    ModbusCommSettings *m_modbusCommSettings;
    void updateStatusBar();
    QLabel *m_statusText;
    QLabel *m_statusInd;
    QLabel *m_statusPackets;
    QLabel *m_statusErrors;
    ModbusAdapter *m_modbus;
    void modbusConnect(bool connect);

private slots:
    void showSettingsModbusRTU();
    void showSettingsModbusTCP();
    void showSettings();
    void showBusMonitor();
    void changedModbusMode(int currIndex);
    void changedFunctionCode(int currIndex);
    void changedBase(int currIndex);
    void changedScanRate(int value);
    void changedConnect(bool value);
    void changedStartAddress(int value);
    void changedNoOfRegs(int value);
    void changedSlaveIP();
    void addItems();
    void clearItems();
    void scan(bool value);
    void request();
    void refreshView();

    void on_HexCombo_activated(int index);

signals:
    void resetCounters();

};

#endif // MAINWINDOW_H
