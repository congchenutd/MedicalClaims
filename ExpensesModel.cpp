#include "ExpensesModel.h"
#include "PageProviders.h"
#include <QDate>

ExpensesModel::ExpensesModel(QObject* parent)
    : MyModel(parent)
{
    setTable("Expense");
    setRelation(COL_PATIENT,    QSqlRelation("Patient",  "ID", "Name"));
    setRelation(COL_PROVIDER,   QSqlRelation("Provider", "ID", "Name"));
    setHeaderData(COL_PATIENT,  Qt::Horizontal, tr("Patient"));
    setHeaderData(COL_PROVIDER, Qt::Horizontal, tr("Provider"));

    initAutoFillRules(COL_COUNT, COL_COUNT);
    addAutoFillRule(new AutoFillMyResponsibility (this, COL_BILLED,         COL_MY_RESPONSIBILITY));
    addAutoFillRule(new AutoFillMyResponsibility (this, COL_INSURANCE_PAID, COL_MY_RESPONSIBILITY));
    addAutoFillRule(new AutoFillMyResponsibility (this, COL_NOT_COVERED,    COL_MY_RESPONSIBILITY));
    addAutoFillRule(new AutoFillMyResponsibility (this, COL_DEDUCTIBLE,     COL_MY_RESPONSIBILITY));
    addAutoFillRule(new AutoFillMyResponsibility (this, COL_COINSURANCE,    COL_MY_RESPONSIBILITY));
    addAutoFillRule(new AutoFillServiceEnd       (this));
//    addAutoFillRule(new AutoFillIPaid            (this));
    addAutoFillRule(new AutoFillTaxable          (this, COL_I_PAID,         COL_TAXABLE));
    addAutoFillRule(new AutoFillTaxable          (this, COL_FSA_CLAIMED,    COL_TAXABLE));
    addAutoFillRule(new AutoFillTaxable          (this, COL_HSA_CLAIMED,    COL_TAXABLE));
}

QVariant ExpensesModel::data(const QModelIndex& idx, int role) const
{
    if (!idx.isValid())
        return QVariant();

    if (role == Qt::TextColorRole)
    {
        if (idx.column() == COL_I_PAID)
        {
            double myResponsibility = data(index(idx.row(), COL_MY_RESPONSIBILITY)) .toDouble();
            double iPaid            = data(index(idx.row(), COL_I_PAID))            .toDouble();
            if (iPaid < myResponsibility)   // unpaid
                return QColor(Qt::red);
        }

        if (idx.column() == COL_FSA_CLAIMED || idx.column() == COL_HSA_CLAIMED)
        {
            double iPaid    = data(index(idx.row(), COL_I_PAID))        .toDouble();
            double fsa      = data(index(idx.row(), COL_FSA_CLAIMED))   .toDouble();
            double hsa      = data(index(idx.row(), COL_HSA_CLAIMED))   .toDouble();
            if (fsa + hsa < iPaid)  // unfiled
                return QColor(Qt::red);
        }
    }

    return QSqlRelationalTableModel::data(idx, role);
}
