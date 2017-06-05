#include "AutoFillRule.h"
#include "ClaimsModel.h"
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
    double notCovered   = _model->data(_model->index(row, ClaimsModel::COL_NOT_COVERED)).toDouble();
    double deductible   = _model->data(_model->index(row, ClaimsModel::COL_DEDUCTIBLE )).toDouble();
    double coinsurance  = _model->data(_model->index(row, ClaimsModel::COL_COINSURANCE)).toDouble();
    double myResponsibility = notCovered + deductible + coinsurance;
    if (myResponsibility > 0) {
        _model->setData(_model->index(row, ClaimsModel::COL_MY_RESPONSIBILITY), myResponsibility);
    }
    else
    {
        double billed        = _model->data(_model->index(row, ClaimsModel::COL_BILLED)).toDouble();
        double insurancePaid = _model->data(_model->index(row, ClaimsModel::COL_INSURANCE_PAID)).toDouble();
        _model->setData(_model->index(row, ClaimsModel::COL_MY_RESPONSIBILITY), billed - insurancePaid);
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
    : AutoFillByCopy (model, ClaimsModel::COL_SERVICE_START, ClaimsModel::COL_SERVICE_END) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillIPaid::AutoFillIPaid(QSqlTableModel* model)
    : AutoFillByCopy (model, ClaimsModel::COL_MY_RESPONSIBILITY, ClaimsModel::COL_I_PAID) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillFSA::AutoFillFSA(QSqlTableModel* model)
    : AutoFillByCopy (model, ClaimsModel::COL_I_PAID, ClaimsModel::COL_FSA_CLAIMED) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
AutoFillHSA::AutoFillHSA(QSqlTableModel* model)
    : AutoFillByCopy (model, ClaimsModel::COL_I_PAID, ClaimsModel::COL_HSA_CLAIMED) {}
