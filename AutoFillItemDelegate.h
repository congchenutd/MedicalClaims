#ifndef AUTOFILLITEMDELEGATE_H
#define AUTOFILLITEMDELEGATE_H

#include "AutoFillRule.h"
#include <QStyledItemDelegate>

class MyModel;

/**
 * An item delegate that can apply auto fill rules using the cell as the source
 */
class AutoFillItemDelegate : public QStyledItemDelegate
{
public:
    AutoFillItemDelegate(MyModel* model, QObject* parent = 0);

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

private:
    void applyRules(int sourceCol, int row) const;

private:
    MyModel* _model;
};

#endif // AUTOFILLITEMDELEGATE_H
