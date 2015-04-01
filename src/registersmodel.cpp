#include "registersmodel.h"
#include "QsLog.h"
#include <QStandardItem>
#include <QtDebug>

#include "eutils.h"

RegistersModel::RegistersModel(QObject *parent) :
    QObject(parent)
{
   model = new  QStandardItemModel(0,0,this);
   m_regDataDelegate = new RegistersDataDelegate(0);
   m_noOfItems = 0;
   m_is16Bit = false;
   clear();
}

void RegistersModel::addItems(int startAddress, int noOfItems, bool valueIsEditable)
{
    int row;
    int col;
    m_startAddress = startAddress;
    m_noOfItems = noOfItems;
    m_offset = (startAddress % 10);
    m_firstRow = startAddress / 10;
    m_lastRow = (startAddress + noOfItems - 1) / 10;

    QLOG_INFO() <<  "Registers Model Address = " << startAddress << " , noOfItems = " << noOfItems
                << " , offset = " << m_offset << " , first row = " << m_firstRow << " , last row = " << m_lastRow;

    //Format Vertical - Horizontal Header
    clear();
    if (noOfItems > 1) {
        model->setHorizontalHeaderLabels(QStringList()<<RegModelHeaderLabels[0]<<RegModelHeaderLabels[1]
                                                    <<RegModelHeaderLabels[2]<<RegModelHeaderLabels[3]
                                                    <<RegModelHeaderLabels[4]<<RegModelHeaderLabels[5]
                                                    <<RegModelHeaderLabels[6]<<RegModelHeaderLabels[7]
                                                    <<RegModelHeaderLabels[8]<<RegModelHeaderLabels[9]);

        QStringList vertHeader;
        for (int i = m_firstRow; i <= m_lastRow ; i++) {
            vertHeader<<QString("%1").arg(i * 10, 2, 10, QLatin1Char('0'));
        }
        model->setVerticalHeaderLabels(vertHeader);
    }
    else {
        model->setHorizontalHeaderLabels(QStringList()<<RegModelHeaderLabels[0]);
        model->setVerticalHeaderLabels(QStringList()<<QString("%1").arg(startAddress, 2, 10, QLatin1Char('0')));
    }

    //Add data to model
    if (noOfItems == 1){
        QStandardItem *valueItem = new QStandardItem("-");model->setItem(0, 0, valueItem);
        valueItem->setEditable(valueIsEditable);
    }
    else {
        for (int i = 0; i < ((m_offset + noOfItems - 1) / 10 + 1) * 10 ; i++) {
            row = i / 10;
            col = i % 10;
            //Address
            if (i >= m_offset + noOfItems || i < m_offset){//not used cells
                QStandardItem *valueItem = new QStandardItem("x");model->setItem(row, col, valueItem);
                valueItem->setEditable(false);
                valueItem->setForeground(QBrush(Qt::red));
                valueItem->setBackground(QBrush(Qt::lightGray));
            }
            else {
                QStandardItem *valueItem = new QStandardItem("-");model->setItem(row, col, valueItem);
                valueItem->setEditable(valueIsEditable);
            }
        }
    }


    emit(refreshView());

}

void RegistersModel::setNoValidValues()
{

    int row;
    int col;
    //if we have no valid values we set  as value = '-/-'

    for (int i = m_offset; i < m_offset + m_noOfItems; i++){
        row = i / 10;
        col = i % 10;
        QModelIndex index = model->index(row, col, QModelIndex());
        model->setData(index,QBrush(Qt::red),Qt::ForegroundRole);
        model->setData(index,"-/-",Qt::DisplayRole);
    }

}

void RegistersModel::setValue(int idx, int value)
{
    int row;
    int col;
    QString convertedValue;

    convertedValue = EUtils::formatValue(value, m_frmt, m_is16Bit);

    //set model data
    if (m_noOfItems == 1){
        row = 0;
        col = 0;
    }
    else {
        row = (m_offset + idx) / 10;
        col = (m_offset + idx) % 10;
    }

    QModelIndex index = model->index(row, col, QModelIndex());
    model->setData(index,QBrush(Qt::black),Qt::ForegroundRole);
    model->setData(index,convertedValue,Qt::DisplayRole);
    model->setData(index,QString("Address : %1").arg(m_startAddress + idx),Qt::ToolTipRole);

}

int RegistersModel::value(int idx)
{
    QString stringVal;
    int intVal;
    bool ok;

    //Get Value
    stringVal = strValue(idx);
    intVal = stringVal.toInt(&ok,m_base);
    if (ok)
        return intVal;
    else
        return -1;

}

QString RegistersModel::strValue(int idx)
{
    int row;
    int col;

    //get model data
    if (m_noOfItems == 1){
        row = 0;
        col = 0;
    }
    else {
        row = (m_offset + idx) / 10;
        col = (m_offset + idx) % 10;
    }
    QModelIndex index = model->index(row, col, QModelIndex());
    QVariant value = model->data(index,Qt::DisplayRole);
    if (value.canConvert<QString>())
        return value.toString();
    else
        return "-/-";

}

void RegistersModel::changeBase(int frmt)
{

    QString stringVal;
    int intVal;
    int row;
    int col;
    bool ok;
    QString convertedVal;

    QLOG_INFO()<<  "Registers Model changed base from " << m_base << " to " << frmt ;

    //change base
    for (int idx = 0; idx < m_noOfItems ; idx++) {
        //Get Value
        stringVal = strValue(idx);
        intVal = stringVal.toInt(&ok,m_base);
        //Format Value
        if (ok)
            convertedVal = EUtils::formatValue(intVal, frmt, m_is16Bit);
        else
            convertedVal = "-/-";
        //Update
        if (m_noOfItems == 1){
            row = 0;
            col = 0;
        }
        else {
            row = (m_offset + idx) / 10;
            col = (m_offset + idx) % 10;
        }
        QModelIndex index = model->index(row, col, QModelIndex());
        model->setData(index,convertedVal,Qt::DisplayRole);
    }

    emit(refreshView());

}

void RegistersModel::clear()
{

    QLOG_INFO()<<  "Registers Model Cleared" ;

    //Clear model
    model->clear();

}

void RegistersModel::setBase(int frmt)
{

    QLOG_INFO()<<  "Registers Model set base = " << frmt ;

    m_regDataDelegate->setBase(frmt);
    changeBase(frmt);
    m_base = frmt;
    m_frmt = frmt;

}

void RegistersModel::setIs16Bit(bool is16Bit)
{

    QLOG_INFO()<<  "Registers Model Is16Bit = " << is16Bit ;
    m_is16Bit = is16Bit;
    m_regDataDelegate->setIs16Bit(is16Bit);

}

RegistersDataDelegate* RegistersModel::itemDelegate()
{

    return m_regDataDelegate;

}
