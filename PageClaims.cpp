#include "PageClaims.h"
#include "DAO.h"
#include "DateDelegate.h"
#include "DlgAttachment.h"
#include "ClaimsModel.h"
#include "MyResponsibilityDelegate.h"
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

    auto delegateNotCovered     = new MyResponsibilityDelegate(ui.tableView);
    auto delegateDeductible     = new MyResponsibilityDelegate(ui.tableView);
    auto delegateCoinsurance    = new MyResponsibilityDelegate(ui.tableView);

    ui.tableView->setItemDelegateForColumn(ClaimsModel::COL_NOT_COVERED,    delegateNotCovered);
    ui.tableView->setItemDelegateForColumn(ClaimsModel::COL_DEDUCTIBLE,     delegateDeductible);
    ui.tableView->setItemDelegateForColumn(ClaimsModel::COL_COINSURANCE,    delegateDeductible);

    connect(delegateNotCovered,     &QItemDelegate::commitData, this, &PageClaims::onDataChanged);
    connect(delegateDeductible,     &QItemDelegate::commitData, this, &PageClaims::onDataChanged);
    connect(delegateCoinsurance,    &QItemDelegate::commitData, this, &PageClaims::onDataChanged);

    ui.widgetAttachments->show();

    connect(ui.tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &PageClaims::onSelectionChanged);
}

void PageClaims::add()
{
    int lastRow = _model->rowCount();
    _model->insertRow(lastRow);
    _model->initRow(lastRow);   // default data
    ui.tableView->edit(_model->index(lastRow, COL_ID + 1));
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
    QSet<int> rows;
    foreach (auto idx, ui.tableView->selectionModel()->selectedIndexes())
        rows << idx.row();

    foreach (auto row, rows)
    {
        QStringList sections;
        for (int col = 0; col < _model->columnCount(); ++col)
            sections << _model->data(_model->index(row, col)).toString();
        os << sections.join(", ") << "\n";
    }
}

void PageClaims::onSelectionChanged(const QItemSelection& selected)
{
    int claimID = selected.isEmpty() ? -1 : _model->data(_model->index(_currentRow, COL_ID)).toInt();
    ui.widgetAttachments->setClaimID(claimID);
}

void PageClaims::onDataChanged()
{
    double notCovered   = _model->data(_model->index(_currentRow, ClaimsModel::COL_NOT_COVERED)).toDouble();
    double deductible   = _model->data(_model->index(_currentRow, ClaimsModel::COL_DEDUCTIBLE)).toDouble();
    double coinsurance  = _model->data(_model->index(_currentRow, ClaimsModel::COL_COINSURANCE)).toDouble();
    double myResponsibility = notCovered + deductible + coinsurance;
    _model->setData(_model->index(_currentRow, ClaimsModel::COL_MY_RESPONSIBILITY), myResponsibility);
}
