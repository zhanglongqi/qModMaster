#ifndef BUSMONITOR_H
#define BUSMONITOR_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
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
    void parseTxPDU(QStringList pdu);
    void parseRxMsg(QString msg);
    void parseRxPDU(QStringList pdu);
    void parseSysMsg(QString msg);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void clear();
    void exit();
    void save();
    void startStop(bool en);
    void selectedRow(const QModelIndex & selected);

};

#endif // BUSMONITOR_H
