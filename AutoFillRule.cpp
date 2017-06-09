#include "AutoFillRule.h"
#include "ExpensesModel.h"
#include <QSqlTableModel>
#include <QDate>

AutoFillRule::AutoFillRule(QSqlTableModel* model, int sourceCol, int destinationCol)
    : _model(model),
      _sourceCol(sourceCol),
      _destinationCol(destinationCol)
{}

AutoFillRule::~AutoFillRule() {}

int AutoFillRule::getSourceColumn() const {
    return _sourceCol;
}

int AutoFillRule::getDestinationColumn() const {
    return _destinationCol;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillMyResponsibility::AutoFillMyResponsibility(QSqlTableModel* model, int sourceCol, int destinationCol)
    : AutoFillRule (model, sourceCol, destinationCol) {}

void AutoFillMyResponsibility::apply(int row)
{
    double notCovered   = _model->data(_model->index(row, ExpensesModel::COL_NOT_COVERED)).toDouble();
    double deductible   = _model->data(_model->index(row, ExpensesModel::COL_DEDUCTIBLE )).toDouble();
    double coinsurance  = _model->data(_model->index(row, ExpensesModel::COL_COINSURANCE)).toDouble();
    double myResponsibility = notCovered + deductible + coinsurance;
    if (myResponsibility > 0) {
        _model->setData(_model->index(row, ExpensesModel::COL_MY_RESPONSIBILITY), myResponsibility);
    }
    else
    {
        double billed        = _model->data(_model->index(row, ExpensesModel::COL_BILLED)).toDouble();
        double insurancePaid = _model->data(_model->index(row, ExpensesModel::COL_INSURANCE_PAID)).toDouble();
        _model->setData(_model->index(row, ExpensesModel::COL_MY_RESPONSIBILITY), billed - insurancePaid);
    }
    _model->submit();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillByCopy::AutoFillByCopy(QSqlTableModel* model, int sourceCol, int destinationCol):
    AutoFillRule (model, sourceCol, destinationCol)
{}

void AutoFillByCopy::apply(int row)
{
    QVariant sourceValue = _model->data(_model->index(row, _sourceCol));
    _model->setData(_model->index(row, _destinationCol), sourceValue);
    _model->submit();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillServiceEnd::AutoFillServiceEnd(QSqlTableModel* model)
    : AutoFillByCopy (model, ExpensesModel::COL_SERVICE_START, ExpensesModel::COL_SERVICE_END) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillIPaid::AutoFillIPaid(QSqlTableModel* model)
    : AutoFillByCopy (model, ExpensesModel::COL_MY_RESPONSIBILITY, ExpensesModel::COL_I_PAID) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillFSA::AutoFillFSA(QSqlTableModel* model)
    : AutoFillByCopy (model, ExpensesModel::COL_I_PAID, ExpensesModel::COL_FSA_CLAIMED) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillHSA::AutoFillHSA(QSqlTableModel* model)
    : AutoFillByCopy (model, ExpensesModel::COL_I_PAID, ExpensesModel::COL_HSA_CLAIMED) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillTaxable::AutoFillTaxable(QSqlTableModel* model, int sourceCol, int destinationCol)
    : AutoFillRule (model, sourceCol, destinationCol) {}

void AutoFillTaxable::apply(int row)
{
    double iPaid    = _model->data(_model->index(row, ExpensesModel::COL_I_PAID)).toDouble();
    double fsa      = _model->data(_model->index(row, ExpensesModel::COL_FSA_CLAIMED)).toDouble();
    double hsa      = _model->data(_model->index(row, ExpensesModel::COL_HSA_CLAIMED)).toDouble();
    _model->setData(_model->index(row, ExpensesModel::COL_TAXABLE), iPaid - fsa - hsa);
    _model->submit();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillRuleDictionary::AutoFillRuleDictionary(int rowCount, int columnCount)
    : _rules (rowCount, QVector<AutoFillRule*>(columnCount, 0))
{
}

void AutoFillRuleDictionary::addRule(AutoFillRule* rule)
{
    int sourceColumn        = rule->getSourceColumn();
    int destinationColumn   = rule->getDestinationColumn();
    if (0 < sourceColumn && sourceColumn < _rules.length() &&
        0 < destinationColumn && destinationColumn < _rules.front().length())
        _rules[sourceColumn][destinationColumn] = rule;
}

QList<AutoFillRule*> AutoFillRuleDictionary::findRulesForSource(int sourceColumn) const
{
    QList<AutoFillRule*> result;
    for (int destinationCol = 0; destinationCol < _rules.length(); ++destinationCol)
    {
        auto rule = _rules[sourceColumn][destinationCol];
        if (rule != 0)
            result << rule;
    }
    return result;
}

QList<AutoFillRule*> AutoFillRuleDictionary::findRulesForDestination(int destinationColumn) const
{
    QList<AutoFillRule*> result;
    for (int sourceCol = 0; sourceCol < _rules.length(); ++sourceCol)
    {
        auto rule = _rules[sourceCol][destinationColumn];
        if (rule != 0)
            result << rule;
    }
    return result;
}
