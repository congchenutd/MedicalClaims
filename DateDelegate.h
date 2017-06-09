#ifndef DATEDELEGATE_H
#define DATEDELEGATE_H

#include "AutoFillItemDelegate.h"

class DateDelegate : public AutoFillItemDelegate
{
public:
    DateDelegate(const AutoFillRuleDictionary& autoFillRules, QObject* parent = 0);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif // DATEDELEGATE_H
