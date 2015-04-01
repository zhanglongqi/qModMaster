#ifndef RAWDATAMODEL_H
#define RAWDATAMODEL_H

#include <QObject>
#include <QStringListModel>

class RawDataModel : public QObject
{
    Q_OBJECT
public:
    explicit RawDataModel(QObject *parent = 0);

    QStringListModel *model;
    void addLine(QString line);
    void enableAddLines(bool en);
    void clear();
    void setMaxNoOfLines(int noOfLines);
    int maxNoOfLines() { return m_maxNoOfLines; }

signals:

public slots:

private:
    QStringList m_rawData;
    int m_maxNoOfLines;
    bool m_addLinesEnabled;

};

#endif // RAWDATAMODEL_H
