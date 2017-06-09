#include "AutoFillItemDelegate.h"

AutoFillItemDelegate::AutoFillItemDelegate(const AutoFillRuleDictionary& autoFillRules, QObject* parent)
    : QStyledItemDelegate(parent),
      _autoFillRules (autoFillRules)
{
}

void AutoFillItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
    applyRules(index.column(), index.row());    // auto fill
}

void AutoFillItemDelegate::applyRules(int sourceCol, int row) const
{
    foreach (auto rule, _autoFillRules.findRulesForSource(sourceCol))
    {
        rule->apply(row);
        applyRules(rule->getDestinationColumn(), row);  // forward actions to downstream columns
    }
}
