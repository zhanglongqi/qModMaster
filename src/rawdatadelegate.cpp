#include "rawdatadelegate.h"
#include <QtDebug>
#include <QPainter>

//TODO : Use delegate
void RawDataDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{

    QStyledItemDelegate::paint(painter, option, index);

}
