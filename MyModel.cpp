#include "MyModel.h"
#include "DAO.h"
#include "PagePatients.h"
#include "PageProviders.h"

#include <QDate>
#include <QSqlQuery>

MyModel::MyModel(QObject* parent)
    : QSqlRelationalTableModel (parent) {}

/**
 * Find the ID of the first record of a foreign table
 */
int MyModel::defaultForeignID(const QString& foreignTableName)
{
    QList<int> foreignIDs = DAO::getInstance()->getIDs(foreignTableName);
    return foreignIDs.isEmpty() ? 0 : foreignIDs.front();
}

void MyModel::initAutoFillRules(int rowCount, int columnCount) {
    _autoFillRules.init(rowCount, columnCount);
}

/**
 * Set initial values of a row
 */
void MyModel::initRow(int row)
{
    setData(index(row, COL_ID), DAO::getNextID(tableName()));

    QVariant data;
    QSqlQuery query(tr("PRAGMA table_info(%1)").arg(tableName()));  // get table schema
    while (query.next())
    {
        auto col        = query.value(0).toInt();
        auto typeName   = query.value(2).toString();
        if (col > COL_ID)
        {
            // Set value based on column type
            if (auto relationModel = this->relationModel(col))
                data = defaultForeignID(relationModel->tableName());
            else if (typeName.compare("double", Qt::CaseInsensitive) == 0)
                data = 0.0;
            else if (typeName.compare("date", Qt::CaseInsensitive) == 0)
                data = QDate::currentDate().toString("yyyy-MM-dd");
            else
                data = QString();
            setData(index(row, col), data);
        }
    }
}

/**
 * Find the value of a given foreign key
 * Useful for finding the raw data of a relational model
 * @param row           - row of the table
 * @param col           - column of the table
 * @param foreignCol    - column of the foreign table
 * @return              - value of the foreign cell
 */
QVariant MyModel::foreignKeyValue(int row, int col, int foreignCol) const
{
    QString displayedValue = data(index(row, col)).toString();
    QSqlTableModel* foreignModel = relationModel(col);
    if (foreignModel == 0)  // not relational model
        return displayedValue;

    // Search for the display value in the foreign table
    QModelIndexList matches = foreignModel->match(foreignModel->index(0, foreignCol),
                                                  Qt::DisplayRole,
                                                  displayedValue,
                                                  1, Qt::MatchExactly | Qt::MatchWrap);
    if (!matches.isEmpty())
    {
        QModelIndex foreignIndex = matches.front();
        return foreignModel->data(foreignModel->index(foreignIndex.row(), COL_ID)).toInt(); // return foreign ID
    }

    return displayedValue;
}

void MyModel::copyRow(int sourceRow, int destinationRow)
{
    for (int col = COL_ID + 1; col < columnCount(); ++col)
    {
        auto relation = this->relation(col);
        if (relation.isValid())
        {
            int foreignCol = relationModel(col)->fieldIndex(relation.displayColumn());
            int foreignID = foreignKeyValue(sourceRow, col, foreignCol).toInt();
            setData(index(destinationRow, col), foreignID);
        }
        else
            setData(index(destinationRow, col), data(index(sourceRow, col)));
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
