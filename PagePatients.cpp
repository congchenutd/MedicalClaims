#include "MyModel.h"
#include "PagePatients.h"

#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QItemEditorFactory>

PagePatients::PagePatients(QWidget* parent) :
    PageDefault(parent)
{
    auto model = new MyModel;
    model->setTable("Patient");
    setModel(model);
    ui.tableView->sortByColumn(COL_NAME, Qt::AscendingOrder);
}

void PagePatients::onSwitch()
{
    ui.splitterHorizontal->setSizes(QList<int>() << 100 << 0);
}
