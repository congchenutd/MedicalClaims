#include "PageExpenses.h"
#include "DAO.h"
#include "DateDelegate.h"
#include "DlgAttachment.h"
#include "ExpensesModel.h"
#include "MyResponsibilityDelegate.h"
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
    _model = new ExpensesModel(this);
    setModel(_model);

    ui.tableView->setAcceptDrops(true);
    ui.tableView->sortByColumn(ExpensesModel::COL_SERVICE_START, Qt::DescendingOrder);
    ui.tableView->getTableHeader()->generateFilters();
    setShowFilter(false);

    ui.tableView->setItemDelegateForColumn(ExpensesModel::COL_PATIENT,        new QSqlRelationalDelegate(ui.tableView));
    ui.tableView->setItemDelegateForColumn(ExpensesModel::COL_PROVIDER,       new QSqlRelationalDelegate(ui.tableView));
    ui.tableView->setItemDelegateForColumn(ExpensesModel::COL_SERVICE_START,  new DateDelegate(ui.tableView));
    ui.tableView->setItemDelegateForColumn(ExpensesModel::COL_SERVICE_END,    new DateDelegate(ui.tableView));

    ui.widgetAttachments->show();

    connect(ui.tableView, &FilterableTableView::attachmentDropped, ui.widgetAttachments, &WidgetAttachments::onDropAttachment);
    connect(ui.tableView->getTableHeader(), &FilterTableHeader::filterChanged, this, &PageExpenses::onFilterChanged);

    _autoFillRules.insert(ExpensesModel::COL_MY_RESPONSIBILITY, new AutoFillMyResponsibility(_model));
    _autoFillRules.insert(ExpensesModel::COL_SERVICE_END,       new AutoFillServiceEnd(_model));
    _autoFillRules.insert(ExpensesModel::COL_I_PAID,            new AutoFillIPaid(_model));
    _autoFillRules.insert(ExpensesModel::COL_FSA_CLAIMED,       new AutoFillFSA(_model));
    _autoFillRules.insert(ExpensesModel::COL_HSA_CLAIMED,       new AutoFillHSA(_model));
    _autoFillRules.insert(ExpensesModel::COL_TAXABLE,           new AutoFillTaxable(_model));
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

void PageExpenses::autoFill()
{
    foreach (auto index, getSelectedIndexes())
        if (AutoFillRule* autoFill = _autoFillRules[index.column()])
            autoFill->apply(index.row());
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

void PageExpenses::initRow(int row) {
    _model->initRow(row);
}

void PageExpenses::copyRow(int sourceRow, int destinationRow) {
    _model->copyRow(sourceRow, destinationRow);
}

void PageExpenses::onFilterChanged(int column, const QString& filterValue)
{
    _model->filterData(column, filterValue);
}
