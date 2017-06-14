#include "AutoFillItemDelegate.h"
#include "MyModel.h"

AutoFillItemDelegate::AutoFillItemDelegate(MyModel* model, QObject* parent)
    : QStyledItemDelegate(parent),
      _model(model)
{
}

void AutoFillItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
    _model->applyAutoFillRules(index.column(), index.row(), true);
}
