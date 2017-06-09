#ifndef MYMODEL_H
#define MYMODEL_H

#include "AutoFillRule.h"

#include <QSqlRelationalTableModel>

/**
 * An item model that supports filtering and auto fill
 */
class MyModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    MyModel(QObject* parent = 0);

    void initRow(int row);
    QVariant foreignKeyValue(int row, int col, int foreignCol) const;
    void copyRow(int sourceRow, int destinationRow);
    bool select() override;

    AutoFillRuleDictionary getAutoFillRules() const;
    void addAutoFillRule(AutoFillRule* rule);
    void autoFill(const QModelIndexList& indexes);

public slots:
    void filterData(int column, const QString& filter);

protected:
    static int defaultForeignID(const QString& foreignTableName);
    void initAutoFillRules(int rowCount, int columnCount);

public:
    enum {COL_ID};

private:
    AutoFillRuleDictionary _autoFillRules;
};

#endif // MYMODEL_H
