#include "ClaimsModel.h"
#include "DAO.h"
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
