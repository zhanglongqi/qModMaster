#ifndef REGISTERSDELEGATE_H
#define REGISTERSDELEGATE_H

#include <QStyledItemDelegate>

class RegistersDataDelegate : public QStyledItemDelegate
 {
     Q_OBJECT

 private:
    int m_base;
    int m_frmt;
    bool m_is16Bit;

 public:
     RegistersDataDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) { }

     void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;

     QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

     void setEditorData(QWidget *editor, const QModelIndex &index) const;
     void setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const;

     void updateEditorGeometry(QWidget *editor,
         const QStyleOptionViewItem &option, const QModelIndex &index) const;

     void setBase(int frmt);
     void setIs16Bit(bool is16Bit);

 };

#endif // REGISTERSDELEGATE_H
