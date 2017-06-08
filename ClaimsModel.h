#ifndef CLAIMSMODEL_H
#define CLAIMSMODEL_H

#include <QSqlRelationalTableModel>

class ClaimsModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    explicit ClaimsModel(QObject* parent = 0);

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    void initRow(int row);
    QVariant foreignKeyValue(int row, int col, int foreignCol) const;
    void copyRow(int sourceRow, int destinationRow);

    bool select() override;

    void filterData(int column, const QString& filter);

public:
    enum {
        COL_ID, COL_PATIENT, COL_PROVIDER, COL_SERVICE, COL_SERVICE_START, COL_SERVICE_END,
        COL_BILLED, COL_COVERED, COL_INSURANCE_PAID, COL_NOT_COVERED, COL_DEDUCTIBLE,
        COL_COINSURANCE, COL_MY_RESPONSIBILITY, COL_I_PAID, COL_FSA_CLAIMED, COL_HSA_CLAIMED};
};

#endif // CLAIMSMODEL_H
