#ifndef RAWDATADELEGATE_H
#define RAWDATADELEGATE_H

#include <QStyledItemDelegate>

class RawDataDelegate : public QStyledItemDelegate
 {
     Q_OBJECT

 public:
     RawDataDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

     void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;

 };
#endif // RAWDATADELEGATE_H
