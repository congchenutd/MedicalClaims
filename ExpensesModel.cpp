#include "ExpensesModel.h"
#include "DAO.h"
#include "PagePatients.h"
#include "PageProviders.h"

#include <QDate>
#include <QIcon>

ExpensesModel::ExpensesModel(QObject* parent)
    : QSqlRelationalTableModel(parent),
    _autoFillRules (COL_COUNT, COL_COUNT)
{
    setTable("Expense");
    setRelation(COL_PATIENT,    QSqlRelation("Patient",  "ID", "Name"));
    setRelation(COL_PROVIDER,   QSqlRelation("Provider", "ID", "Name"));
    setHeaderData(COL_PATIENT,  Qt::Horizontal, tr("Patient"));
    setHeaderData(COL_PROVIDER, Qt::Horizontal, tr("Provider"));

    _autoFillRules.addRule(new AutoFillMyResponsibility (this, COL_BILLED,         COL_MY_RESPONSIBILITY));
    _autoFillRules.addRule(new AutoFillMyResponsibility (this, COL_INSURANCE_PAID, COL_MY_RESPONSIBILITY));
    _autoFillRules.addRule(new AutoFillMyResponsibility (this, COL_NOT_COVERED,    COL_MY_RESPONSIBILITY));
    _autoFillRules.addRule(new AutoFillMyResponsibility (this, COL_DEDUCTIBLE,     COL_MY_RESPONSIBILITY));
    _autoFillRules.addRule(new AutoFillMyResponsibility (this, COL_COINSURANCE,    COL_MY_RESPONSIBILITY));
    _autoFillRules.addRule(new AutoFillServiceEnd       (this));
    _autoFillRules.addRule(new AutoFillIPaid            (this));
    _autoFillRules.addRule(new AutoFillFSA              (this));
    _autoFillRules.addRule(new AutoFillHSA              (this));
    _autoFillRules.addRule(new AutoFillTaxable          (this, COL_FSA_CLAIMED,    COL_TAXABLE));
    _autoFillRules.addRule(new AutoFillTaxable          (this, COL_HSA_CLAIMED,    COL_TAXABLE));
}

QVariant ExpensesModel::data(const QModelIndex& idx, int role) const
{
    if (!idx.isValid())
        return QVariant();

    if (role == Qt::TextColorRole)
    {
        if (idx.column() == COL_I_PAID)
        {
            double myResponsibility = data(index(idx.row(), COL_MY_RESPONSIBILITY)).toDouble();
            double iPaid            = data(index(idx.row(), COL_I_PAID)).toDouble();
            if (iPaid < myResponsibility)
                return QColor(Qt::red);
        }

        if (idx.column() == COL_FSA_CLAIMED || idx.column() == COL_HSA_CLAIMED)
        {
            double iPaid    = data(index(idx.row(), COL_I_PAID)).toDouble();
            double fsa      = data(index(idx.row(), COL_FSA_CLAIMED)).toDouble();
            double hsa      = data(index(idx.row(), COL_HSA_CLAIMED)).toDouble();
            if (fsa + hsa < iPaid)
                return QColor(Qt::red);
        }
    }

    return QSqlRelationalTableModel::data(idx, role);
}

void ExpensesModel::initRow(int row)
{
    setData(index(row, COL_ID), DAO::getNextID(tableName()));

    QList<int> patientIDs = DAO::getInstance()->getIDs("Patient");
    int patientID = patientIDs.isEmpty() ? 0 : patientIDs.front();
    setData(index(row, COL_PATIENT), patientID);

    QList<int> providerIDs = DAO::getInstance()->getIDs("Provider");
    int providerID = providerIDs.isEmpty() ? 0 : providerIDs.front();
    setData(index(row, COL_PROVIDER), providerID);

    setData(index(row, COL_SERVICE_START),  QDate::currentDate());
    setData(index(row, COL_SERVICE_END),    QDate::currentDate());

    for (int col = COL_BILLED; col < rowCount(); ++col)
        setData(index(row, col), 0.0);
}

QVariant ExpensesModel::foreignKeyValue(int row, int col, int foreignCol) const
{
    QString displayedValue = data(index(row, col)).toString();
    QSqlTableModel* foreignModel = relationModel(col);
    if (foreignModel == 0)
        return displayedValue;

    QModelIndexList matches = foreignModel->match(foreignModel->index(0, foreignCol),
                                                  Qt::DisplayRole,
                                                  displayedValue,
                                                  1, Qt::MatchExactly | Qt::MatchWrap);
    if (!matches.isEmpty())
    {
        QModelIndex foreignIndex = matches.front();
        return foreignModel->data(foreignModel->index(foreignIndex.row(), COL_ID)).toInt();
    }

    return displayedValue;
}

void ExpensesModel::copyRow(int sourceRow, int destinationRow)
{
    // copy foreign keys
    int patientID = foreignKeyValue(sourceRow, COL_PATIENT, PagePatients::COL_NAME).toInt();
    setData(index(destinationRow, COL_PATIENT), patientID);

    int providerID = foreignKeyValue(sourceRow, COL_PROVIDER, PageProviders::COL_NAME).toInt();
    setData(index(destinationRow, COL_PROVIDER), providerID);

    // copy the rest
    for (int col = ExpensesModel::COL_SERVICE; col < columnCount(); ++col)
        setData(index(destinationRow, col), data(index(sourceRow, col)));
}

bool ExpensesModel::select()
{
    relationModel(ExpensesModel::COL_PATIENT)->select();
    relationModel(ExpensesModel::COL_PROVIDER)->select();
    return QSqlRelationalTableModel::select();
}

void ExpensesModel::filterData(int column, const QString& filter)
{
    // clear filter and reset model
    if (filter.isEmpty())
    {
        setFilter(filter);
        select();
        return;
    }

    // the column is a foreign key, relTblAl_x is the alias of the foreign table, where x is the column
    if (relationModel(column))
        setFilter(tr("relTblAl_%1.Name LIKE \"%%2%\"")
                  .arg(column)
                  .arg(filter));
    else
        setFilter(tr("\"%1\" LIKE \"%%2%\"")
                  .arg(headerData(column, Qt::Horizontal).toString())
                  .arg(filter));
}

void ExpensesModel::applyRules(const QModelIndexList& indexes)
{
    foreach (auto index, indexes)
    {
        auto rules = _autoFillRules.findRulesForDestination(index.column());
        if (!rules.isEmpty())
            rules.front()->apply(index.row());
    }
}

AutoFillRuleDictionary ExpensesModel::getAutoFillRules() const {
    return _autoFillRules;
}
