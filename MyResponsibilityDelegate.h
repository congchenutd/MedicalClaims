#ifndef MYRESPONSIBILITYDELEGATE_H
#define MYRESPONSIBILITYDELEGATE_H

#include <QStyledItemDelegate>

class MyResponsibilityDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    MyResponsibilityDelegate(QObject* parent = 0);

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

signals:
    void dataCommited() const;
};

#endif // MYRESPONSIBILITYDELEGATE_H
