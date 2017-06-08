#include "AutoFillRule.h"
#include "ExpensesModel.h"
#include <QSqlTableModel>
#include <QDate>

AutoFillRule::AutoFillRule(QSqlTableModel* model)
    : _model(model) {}

AutoFillRule::~AutoFillRule() {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillMyResponsibility::AutoFillMyResponsibility(QSqlTableModel* model)
    : AutoFillRule (model) {}

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
AutoFillByCopy::AutoFillByCopy(QSqlTableModel* model, int sourceColumn, int destinationColumn):
    AutoFillRule (model),
    _sourceColumn (sourceColumn),
    _destinationColumn (destinationColumn) {}

void AutoFillByCopy::apply(int row)
{
    QVariant sourceValue = _model->data(_model->index(row, _sourceColumn));
    _model->setData(_model->index(row, _destinationColumn), sourceValue);
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
AutoFillTaxable::AutoFillTaxable(QSqlTableModel* model)
    : AutoFillRule (model) {}

void AutoFillTaxable::apply(int row)
{
    double iPaid    = _model->data(_model->index(row, ExpensesModel::COL_I_PAID)).toDouble();
    double fsa      = _model->data(_model->index(row, ExpensesModel::COL_FSA_CLAIMED)).toDouble();
    double hsa      = _model->data(_model->index(row, ExpensesModel::COL_HSA_CLAIMED)).toDouble();
    _model->setData(_model->index(row, ExpensesModel::COL_TAXABLE), iPaid - fsa - hsa);
    _model->submit();
}
