#include "PagePatients.h"

#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QItemEditorFactory>

PagePatients::PagePatients(QWidget* parent) :
    PageDefault(parent)
{
    QSqlTableModel* model = new QSqlTableModel;
    model->setTable("Patient");
    setModel(model);
    ui.tableView->sortByColumn(COL_NAME, Qt::AscendingOrder);
}
