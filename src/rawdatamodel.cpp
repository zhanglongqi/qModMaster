#include "rawdatamodel.h"
#include "QsLog.h"
#include <QtDebug>

RawDataModel::RawDataModel(QObject *parent) :
    QObject(parent)
{
    model = new  QStringListModel(this);
    m_addLinesEnabled = false;
}

void RawDataModel::addLine(QString line)
{

    if (!m_addLinesEnabled) return;

    QLOG_INFO() <<  "Raw Data Model Line = " << line << " , No of lines = " << m_rawData.length();

    if (m_rawData.length() == m_maxNoOfLines)
        m_rawData.removeFirst();
    m_rawData.append(line);
    model->setStringList(m_rawData);

}

void RawDataModel::clear()
{

    QLOG_INFO() <<  "Raw Data Model cleared" ;

    m_rawData.clear();
    model->setStringList(m_rawData);

}

void RawDataModel::setMaxNoOfLines(int noOfLines)
{
    m_maxNoOfLines = noOfLines;
}

void RawDataModel::enableAddLines(bool en)
{
    m_addLinesEnabled = en;
}
