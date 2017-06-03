#include "ClaimsModel.h"
#include "DAO.h"
#include "PagePatients.h"
#include "PageProviders.h"

#include <QDate>
#include <QIcon>

ClaimsModel::ClaimsModel(QObject* parent)
    : QSqlRelationalTableModel(parent)
{
    setTable("Claim");
    setRelation(COL_PATIENT,    QSqlRelation("Patient",  "ID", "Name"));
    setRelation(COL_PROVIDER,   QSqlRelation("Provider", "ID", "Name"));
    setHeaderData(COL_PATIENT,  Qt::Horizontal, tr("Patient"));
    setHeaderData(COL_PROVIDER, Qt::Horizontal, tr("Provider"));
}

QVariant ClaimsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

//    if (role == Qt::DecorationRole && index.column() == COL_I_PAID)
//    {
//        return QIcon(":/Images/Check.png");
//    }

    return QSqlRelationalTableModel::data(index, role);
}

void ClaimsModel::initRow(int row)
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

    for (int col = COL_BILLED; col <= COL_HSA_CLAIMED; ++col)
        setData(index(row, col), 0.0);
}

QVariant ClaimsModel::foreignKeyValue(int row, int col, int foreignCol) const
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

void ClaimsModel::copyRow(int sourceRow, int destinationRow)
{
    // copy foreign keys
    int patientID = foreignKeyValue(sourceRow, COL_PATIENT, PagePatients::COL_NAME).toInt();
    setData(index(destinationRow, COL_PATIENT), patientID);

    int providerID = foreignKeyValue(sourceRow, COL_PROVIDER, PageProviders::COL_NAME).toInt();
    setData(index(destinationRow, COL_PROVIDER), providerID);

    // copy the rest
    for (int col = ClaimsModel::COL_SERVICE; col < columnCount(); ++col)
        setData(index(destinationRow, col), data(index(sourceRow, col)));
}
