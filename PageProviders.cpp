#include "MyModel.h"
#include "PageProviders.h"

PageProviders::PageProviders(QWidget* parent) :
    PageDefault(parent)
{
    auto model = new MyModel;
    model->setTable("Provider");
    setModel(model);
    ui.tableView->sortByColumn(COL_NAME, Qt::AscendingOrder);
}
