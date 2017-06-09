#include "MyModel.h"
#include "DAO.h"
#include "PagePatients.h"
#include "PageProviders.h"

#include <QDate>
#include <QSqlQuery>

MyModel::MyModel(QObject* parent)
    : QSqlRelationalTableModel (parent) {}

void MyModel::setTable(const QString& tableName)
{
    QSqlRelationalTableModel::setTable(tableName);

    // init columns' default values
    QVariant defaultValue;
    QSqlQuery query(tr("PRAGMA table_info(%1)").arg(tableName));  // get table schema
    while (query.next())
    {
        auto col        = query.value(0).toInt();
        auto typeName   = query.value(2).toString();
        if (col > COL_ID && relationModel(col) == 0)
        {
            if (typeName.compare("double", Qt::CaseInsensitive) == 0)
                defaultValue = 0.0;
            else if (typeName.compare("date", Qt::CaseInsensitive) == 0)
                defaultValue = QDate::currentDate().toString("yyyy-MM-dd");
            _defaultValues.insert(col, defaultValue);
        }
    }
}

void MyModel::initAutoFillRules(int rowCount, int columnCount) {
    _autoFillRules.init(rowCount, columnCount);
}

QVariant MyModel::getDefaultValue(int col) const
{
    if (col == COL_ID)
        return DAO::getNextID(tableName());

    //  Find the ID of the first record of a foreign table
    if (auto relationModel = this->relationModel(col))
    {
        QList<int> foreignIDs = DAO::getInstance()->getIDs(relationModel->tableName());
        return foreignIDs.isEmpty() ? 0 : foreignIDs.front();
    }

    if (_defaultValues.contains(col))
        return _defaultValues[col];

    return QVariant();
}

/**
 * Set initial values of a row
 */
void MyModel::initRow(int row)
{
    for (int col = COL_ID; col < columnCount(); ++col)
        setData(index(row, col), getDefaultValue(col));
}

void MyModel::resetCell(const QModelIndex& idx)
{
    if (idx.column() > COL_ID)
        setData(idx, getDefaultValue(idx.column()));
}

void MyModel::copyRow(int sourceRow, int destinationRow)
{
    for (int col = COL_ID + 1; col < columnCount(); ++col)
    {
        auto relation = this->relation(col);
        auto idx = index(sourceRow, col);
        if (relation.isValid())
        {
            int foreignID = DAO::getInstance()->getID(relation.tableName(),
                                                      relation.displayColumn(),
                                                      data(idx).toString());
            setData(index(destinationRow, col), foreignID);
        }
        else
            setData(index(destinationRow, col), data(idx));
    }
}

bool MyModel::select()
{
    // update relations
    for (int col = COL_ID + 1; col < columnCount(); ++col)
        if (auto relationModel = this->relationModel(col))
            relationModel->select();
    return QSqlRelationalTableModel::select();
}

void MyModel::filterData(int column, const QString& filter)
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

/**
 * Auto fill the given indexes
 */
void MyModel::autoFill(const QModelIndexList& indexes)
{
    foreach (auto index, indexes)
    {
        auto rules = _autoFillRules.findRulesForDestination(index.column());
        if (!rules.isEmpty())
            rules.front()->apply(index.row());  // only apply the first rule for this column
    }
}

AutoFillRuleDictionary MyModel::getAutoFillRules() const {
    return _autoFillRules;
}

void MyModel::addAutoFillRule(AutoFillRule* rule) {
    _autoFillRules.addRule(rule);
}
