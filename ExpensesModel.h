#ifndef CLAIMSMODEL_H
#define CLAIMSMODEL_H

#include "MyModel.h"

class ExpensesModel : public MyModel
{
public:
    explicit ExpensesModel(QObject* parent = 0);

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;

public:
    enum {
        COL_ID, COL_PATIENT, COL_PROVIDER, COL_SERVICE, COL_SERVICE_START, COL_SERVICE_END,
        COL_BILLED, COL_COVERED, COL_INSURANCE_PAID, COL_NOT_COVERED, COL_DEDUCTIBLE,
        COL_COINSURANCE, COL_MY_RESPONSIBILITY, COL_I_PAID, COL_FSA_CLAIMED, COL_HSA_CLAIMED,
        COL_TAXABLE, COL_NOTES, COL_COUNT};
};

#endif // CLAIMSMODEL_H
