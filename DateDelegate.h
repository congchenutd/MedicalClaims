#ifndef DATEDELEGATE_H
#define DATEDELEGATE_H

#include "AutoFillItemDelegate.h"

/**
 * An item delegate for showing and editing date
 */
class DateDelegate : public AutoFillItemDelegate
{
public:
    DateDelegate(MyModel* model, QObject* parent = 0);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif // DATEDELEGATE_H
