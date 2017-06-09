#include "DateDelegate.h"
#include "ExpensesModel.h"
#include <QDateEdit>

DateDelegate::DateDelegate(const AutoFillRuleDictionary& autoFillRules, QObject* parent)
    : AutoFillItemDelegate (autoFillRules, parent)
{

}

QWidget* DateDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QDateEdit* dateEdit = new QDateEdit(parent);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setCalendarPopup(true); // enable calendar
    return dateEdit;
}

void DateDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QDate date = index.model()->data(index).toDate();
    if (!date.isValid())
        date = QDate::currentDate();
    QDateEdit* dateEdit = qobject_cast<QDateEdit*>(editor);
    dateEdit->setDate(date);
}

void DateDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QDateEdit* dateEdit = qobject_cast<QDateEdit*>(editor);
    QDate date = dateEdit->date();
    model->setData(index, date.toString("yyyy-MM-dd"));
    model->submit();

    AutoFillItemDelegate::setModelData(editor, model, index);
}
