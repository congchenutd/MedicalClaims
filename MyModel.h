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

    void setTable(const QString& tableName) override;

    void initRow(int row);
    void resetCell(const QModelIndex& idx);
    void copyRow(int sourceRow, int destinationRow);
    bool select() override;

    AutoFillRuleDictionary getAutoFillRules() const;
    void addAutoFillRule(AutoFillRule* rule);
    void autoFill(const QModelIndexList& indexes);
    bool submit() override;

public slots:
    void filterData(int column, const QString& filter);

protected:
    void initAutoFillRules(int rowCount, int columnCount);
    QVariant getDefaultValue(int col) const;

signals:
    void dirty(bool isDirty);

public:
    enum {COL_ID};

private:
    AutoFillRuleDictionary  _autoFillRules;
    QMap<int, QVariant>     _defaultValues;
};

#endif // MYMODEL_H
