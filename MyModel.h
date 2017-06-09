#ifndef MYMODEL_H
#define MYMODEL_H

#include <QSqlRelationalTableModel>

class MyModel : public QSqlRelationalTableModel
{
public:
    MyModel(QObject* parent = 0);
};

#endif // MYMODEL_H
