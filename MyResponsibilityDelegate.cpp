#include "MyResponsibilityDelegate.h"

MyResponsibilityDelegate::MyResponsibilityDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{    
}

void MyResponsibilityDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
    emit dataCommited();
}
