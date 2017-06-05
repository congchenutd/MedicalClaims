#include "PageClaims.h"
#include "DAO.h"
#include "DateDelegate.h"
#include "DlgAttachment.h"
#include "ClaimsModel.h"
#include "MyResponsibilityDelegate.h"
#include "PagePatients.h"
#include "PageProviders.h"
#include "AutoFillRule.h"

#include <QDate>
#include <QMetaEnum>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

PageClaims::PageClaims(QWidget* parent) :
    PageDefault(parent)
{
    _model = new ClaimsModel(this);
    initModel(_model);

    ui.tableView->sortByColumn(ClaimsModel::COL_SERVICE_START, Qt::DescendingOrder);

    ui.tableView->setItemDelegateForColumn(ClaimsModel::COL_PATIENT,  new QSqlRelationalDelegate(ui.tableView));
    ui.tableView->setItemDelegateForColumn(ClaimsModel::COL_PROVIDER, new QSqlRelationalDelegate(ui.tableView));

    ui.tableView->setItemDelegateForColumn(ClaimsModel::COL_SERVICE_START, new DateDelegate(ui.tableView));
    ui.tableView->setItemDelegateForColumn(ClaimsModel::COL_SERVICE_END,   new DateDelegate(ui.tableView));

    ui.widgetAttachments->show();

    connect(ui.tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &PageClaims::onSelectionChanged);

    connect(ui.tableView, &TableViewClaims::attachmentDropped, ui.widgetAttachments, &WidgetAttachments::onDropAttachment);

    _autoFillRules.insert(ClaimsModel::COL_MY_RESPONSIBILITY,   new AutoFillMyResponsibility(_model));
    _autoFillRules.insert(ClaimsModel::COL_SERVICE_END,         new AutoFillServiceEnd(_model));
    _autoFillRules.insert(ClaimsModel::COL_I_PAID,              new AutoFillIPaid(_model));
    _autoFillRules.insert(ClaimsModel::COL_FSA_CLAIMED,         new AutoFillFSA(_model));
    _autoFillRules.insert(ClaimsModel::COL_HSA_CLAIMED,         new AutoFillHSA(_model));
}

void PageClaims::exportData(const QString& fileName)
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

void PageClaims::autoFill()
{
    foreach (auto index, getSelectedIndexes())
        if (AutoFillRule* autoFill = _autoFillRules[index.column()])
            autoFill->apply(index.row());
}

void PageClaims::initRow(int row) {
    _model->initRow(row);
}

void PageClaims::copyRow(int sourceRow, int destinationRow) {
    _model->copyRow(sourceRow, destinationRow);
}

void PageClaims::onSelectionChanged(const QItemSelection& selected)
{
    int claimID = selected.isEmpty() ? -1 : _model->data(_model->index(_currentRow, COL_ID)).toInt();
    ui.widgetAttachments->setClaimID(claimID);
}
