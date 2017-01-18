#ifndef BUSMONITOR_H
#define BUSMONITOR_H

#include <QMainWindow>
#include <QLabel>
#include "src/rawdatamodel.h"

namespace Ui {
    class BusMonitor;
}

class BusMonitor : public QMainWindow
{
    Q_OBJECT

public:
    explicit BusMonitor(QWidget *parent, RawDataModel *rawDataModel);
    ~BusMonitor();

private:
    Ui::BusMonitor *ui;
    RawDataModel *m_rawDataModel;
    void parseTxMsg(QString msg);
    void parseTxPDU(QStringList pdu, QString slave);
    void parseRxMsg(QString msg);
    void parseRxPDU(QStringList pdu, QString slave);
    void parseSysMsg(QString msg);

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void clear();
    void exit();
    void save();
    void selectedRow(const QModelIndex & selected);

};

#endif // BUSMONITOR_H
