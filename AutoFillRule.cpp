#include "AutoFillRule.h"
#include "ExpensesModel.h"
#include <QSqlTableModel>
#include <QDate>

/**
 * Auto filling may need multple rules
 * @param model             - underlying model
 * @param sourceCol         - source column
 * @param destinationCol    - destination column
 */
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
        _model->setData(_model->index(row, getDestinationColumn()), myResponsibility);
    }
    else
    {
        double billed        = _model->data(_model->index(row, ExpensesModel::COL_BILLED))          .toDouble();
        double insurancePaid = _model->data(_model->index(row, ExpensesModel::COL_INSURANCE_PAID))  .toDouble();
        _model->setData(_model->index(row, getDestinationColumn()), billed - insurancePaid);
    }
    _model->submit();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillByCopy::AutoFillByCopy(QSqlTableModel* model, int sourceCol, int destinationCol):
    AutoFillRule (model, sourceCol, destinationCol)
{}

void AutoFillByCopy::apply(int row)
{
    QVariant sourceValue = _model->data(_model->index(row, getSourceColumn()));
    _model->setData(_model->index(row, getDestinationColumn()), sourceValue);
    _model->submit();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillServiceEnd::AutoFillServiceEnd(QSqlTableModel* model)
    : AutoFillByCopy (model, ExpensesModel::COL_SERVICE_START, ExpensesModel::COL_SERVICE_END) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillIPaid::AutoFillIPaid(QSqlTableModel* model)
    : AutoFillByCopy (model, ExpensesModel::COL_MY_RESPONSIBILITY, ExpensesModel::COL_I_PAID) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillTaxable::AutoFillTaxable(QSqlTableModel* model, int sourceCol, int destinationCol)
    : AutoFillRule (model, sourceCol, destinationCol) {}

void AutoFillTaxable::apply(int row)
{
    double iPaid    = _model->data(_model->index(row, ExpensesModel::COL_I_PAID))       .toDouble();
    double fsa      = _model->data(_model->index(row, ExpensesModel::COL_FSA_CLAIMED))  .toDouble();
    double hsa      = _model->data(_model->index(row, ExpensesModel::COL_HSA_CLAIMED))  .toDouble();
    _model->setData(_model->index(row, getDestinationColumn()), iPaid - fsa - hsa);
    _model->submit();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void AutoFillRuleDictionary::init(int rowCount, int columnCount) {
    _rules.fill(QVector<AutoFillRule*>(columnCount, 0), rowCount);  // a 2D array: [source col, destination col]
}

void AutoFillRuleDictionary::addRule(AutoFillRule* rule)
{
    int sourceColumn        = rule->getSourceColumn();
    int destinationColumn   = rule->getDestinationColumn();
    if (0 < sourceColumn && sourceColumn < _rules.length() &&
        0 < destinationColumn && destinationColumn < _rules.front().length())
        _rules[sourceColumn][destinationColumn] = rule;
}

/**
 * @return - find all rules with a given source column
 */
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

/**
 * @return - find all rules for a given destination column
 */
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
