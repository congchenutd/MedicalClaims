#ifndef AUTOFILLRULE_H
#define AUTOFILLRULE_H

#include <QVector>

class QSqlTableModel;

/**
 * A rule for auto filling a destination column
 */
class AutoFillRule
{
public:
    AutoFillRule(QSqlTableModel* model, int sourceCol, int destinationCol);
    virtual ~AutoFillRule();

    int getSourceColumn()       const;
    int getDestinationColumn()  const;

    // Apply the rule to the given row
    virtual void apply(int row) = 0;

protected:
    QSqlTableModel* _model;
    int             _sourceCol;
    int             _destinationCol;
};

class AutoFillMyResponsibility: public AutoFillRule
{
public:
    AutoFillMyResponsibility(QSqlTableModel* model, int sourceCol, int destinationCol);
    void apply(int row) override;
};

/**
 * Copy one cell to another
 */
class AutoFillByCopy: public AutoFillRule
{
public:
    AutoFillByCopy(QSqlTableModel* model, int sourceCol, int destinationCol);
    void apply(int row) override;
};

class AutoFillServiceEnd: public AutoFillByCopy
{
public:
    AutoFillServiceEnd(QSqlTableModel* model);
};

class AutoFillIPaid: public AutoFillByCopy
{
public:
    AutoFillIPaid(QSqlTableModel* model);
};

class AutoFillFSA: public AutoFillByCopy
{
public:
    AutoFillFSA(QSqlTableModel* model);
};

class AutoFillHSA: public AutoFillByCopy
{
public:
    AutoFillHSA(QSqlTableModel* model);
};

class AutoFillTaxable: public AutoFillRule
{
public:
    AutoFillTaxable(QSqlTableModel* model, int sourceCol, int destinationCol);
    void apply(int row) override;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
class AutoFillRuleDictionary
{
public:
    void init(int rowCount, int columnCount);
    void addRule(AutoFillRule* rule);
    QList<AutoFillRule*> findRulesForSource     (int sourceColumn) const;
    QList<AutoFillRule*> findRulesForDestination(int destinationColumn) const;

private:
    QVector<QVector<AutoFillRule*>> _rules;
};

#endif // AUTOFILLRULE_H
