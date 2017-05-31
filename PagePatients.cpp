#include "PagePatients.h"

#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QItemEditorFactory>

PagePatients::PagePatients(QWidget* parent) :
    PageDefault(parent)
{
    QSqlTableModel* model = new QSqlTableModel;
    model->setTable("Patient");
    initModel(model);
}
