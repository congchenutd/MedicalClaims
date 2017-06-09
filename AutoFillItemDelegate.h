#ifndef AUTOFILLITEMDELEGATE_H
#define AUTOFILLITEMDELEGATE_H

#include "AutoFillRule.h"
#include <QStyledItemDelegate>

/**
 * An item delegate that can apply auto fill rules using the cell as the source
 */
class AutoFillItemDelegate : public QStyledItemDelegate
{
public:
    AutoFillItemDelegate(const AutoFillRuleDictionary& autoFillRules, QObject* parent = 0);

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

private:
    void applyRules(int sourceCol, int row) const;

private:
    AutoFillRuleDictionary _autoFillRules;
};

#endif // AUTOFILLITEMDELEGATE_H
