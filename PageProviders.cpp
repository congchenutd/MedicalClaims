#include "PageProviders.h"

PageProviders::PageProviders(QWidget* parent) :
    PageDefault(parent)
{
    QSqlTableModel* model = new QSqlTableModel;
    model->setTable("Provider");
    initModel(model);
    ui.tableView->sortByColumn(COL_NAME, Qt::AscendingOrder);
}
