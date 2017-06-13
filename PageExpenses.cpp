#include "PageExpenses.h"
#include "DAO.h"
#include "DateDelegate.h"
#include "DlgAttachment.h"
#include "ExpensesModel.h"
#include "PagePatients.h"
#include "PageProviders.h"
#include "AutoFillRule.h"
#include "FilterTableHeader.h"

#include <QDate>
#include <QMetaEnum>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

PageExpenses::PageExpenses(QWidget* parent) :
    PageDefault(parent)
{
    setModel(new ExpensesModel(this));

    ui.tableView->setAcceptDrops(true);
    ui.tableView->sortByColumn(ExpensesModel::COL_SERVICE_START, Qt::DescendingOrder);
    ui.tableView->getTableHeader()->generateFilters();
    setShowFilter(false);

    // relational delegates
    ui.tableView->setItemDelegateForColumn(ExpensesModel::COL_PATIENT,  new QSqlRelationalDelegate(ui.tableView));
    ui.tableView->setItemDelegateForColumn(ExpensesModel::COL_PROVIDER, new QSqlRelationalDelegate(ui.tableView));

    // auto fill delegates
    auto dateDelegate = new DateDelegate(_model->getAutoFillRules(), ui.tableView);
    ui.tableView->setItemDelegateForColumn(ExpensesModel::COL_SERVICE_START,  dateDelegate);
    ui.tableView->setItemDelegateForColumn(ExpensesModel::COL_SERVICE_END,    dateDelegate);

    auto autoFillDelegate = new AutoFillItemDelegate(_model->getAutoFillRules(), ui.tableView);
    for (int col = ExpensesModel::COL_BILLED; col < ExpensesModel::COL_COUNT; ++col)
        ui.tableView->setItemDelegateForColumn(col, autoFillDelegate);

    ui.widgetAttachments->show();

    connect(ui.tableView, &FilterableTableView::attachmentDropped, ui.widgetAttachments, &WidgetAttachments::onDropAttachment);
    connect(ui.tableView->getTableHeader(), &FilterTableHeader::filtersChanged, _model, &MyModel::onFiltersChanged);
}

void PageExpenses::exportData(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::critical(this, tr("Error"), tr("Unable to save to file %1").arg(fileName));
        return;
    }

    QTextStream os(&file);

    // Header
    QStringList sections;
    for (int col = 0; col < _model->columnCount(); ++col)
        sections << _model->headerData(col, Qt::Horizontal).toString();
    os << sections.join(", ") << "\n";

    // Content
    foreach (auto row, getSelectedRows())
    {
        QStringList sections;
        for (int col = 0; col < _model->columnCount(); ++col)
            sections << _model->data(_model->index(row, col)).toString();
        os << sections.join(", ") << "\n";
    }
}

void PageExpenses::setShowFilter(bool show)
{
    ui.tableView->setShowFilter(show);

    // UGLY: force layout update to show/hide the filters
    adjustSize();
    layout()->update();

    // clear filter
    if (!show)
        _model->setFilter(QString());
}
