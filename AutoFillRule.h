#ifndef AUTOFILLRULE_H
#define AUTOFILLRULE_H

class QSqlTableModel;

class AutoFillRule
{
public:
    AutoFillRule(QSqlTableModel* model);
    virtual ~AutoFillRule();

    virtual void apply(int row) = 0;

protected:
    QSqlTableModel* _model;
};

class AutoFillMyResponsibility: public AutoFillRule
{
public:
    AutoFillMyResponsibility(QSqlTableModel* model);
    void apply(int row) override;
};

class AutoFillByCopy: public AutoFillRule
{
public:
    AutoFillByCopy(QSqlTableModel* model, int sourceColumn, int destinationColumn);
    void apply(int row) override;

protected:
    int _sourceColumn;
    int _destinationColumn;
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
    AutoFillTaxable(QSqlTableModel* model);
    void apply(int row) override;
};

#endif // AUTOFILLRULE_H
