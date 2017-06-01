#include "PageClaims.h"
#include "DAO.h"
#include "DateDelegate.h"
#include "DlgAttachment.h"
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
    QSqlRelationalTableModel* model = new QSqlRelationalTableModel;
    model->setTable("Claim");
    model->setRelation(COL_PATIENT,    QSqlRelation("Patient",  "ID", "Name"));
    model->setRelation(COL_PROVIDER,   QSqlRelation("Provider", "ID", "Name"));
    model->setHeaderData(COL_PATIENT,  Qt::Horizontal, tr("Patient"));
    model->setHeaderData(COL_PROVIDER, Qt::Horizontal, tr("Provider"));
    initModel(model);
    ui.tableView->sortByColumn(COL_SERVICE_START, Qt::DescendingOrder);

    ui.tableView->setItemDelegateForColumn(COL_PATIENT,  new QSqlRelationalDelegate(ui.tableView));
    ui.tableView->setItemDelegateForColumn(COL_PROVIDER, new QSqlRelationalDelegate(ui.tableView));
    ui.tableView->setItemDelegateForColumn(COL_SERVICE_START, new DateDelegate(ui.tableView));
    ui.tableView->setItemDelegateForColumn(COL_SERVICE_END,   new DateDelegate(ui.tableView));

    ui.widgetAttachments->show();

    connect(ui.tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &PageClaims::onSelectionChanged);
}

void PageClaims::add()
{
    int lastRow = _model->rowCount();
    _model->insertRow(lastRow);

    // default data
    _model->setData(_model->index(lastRow, COL_ID), DAO::getNextID(_model->tableName()));

    QList<int> patientIDs = DAO::getInstance()->getIDs("Patient");
    int patientID = patientIDs.isEmpty() ? 0 : patientIDs.front();
    _model->setData(_model->index(lastRow, COL_PATIENT), patientID);

    QList<int> providerIDs = DAO::getInstance()->getIDs("Provider");
    int providerID = providerIDs.isEmpty() ? 0 : providerIDs.front();
    _model->setData(_model->index(lastRow, COL_PROVIDER), providerID);

    _model->setData(_model->index(lastRow, COL_SERVICE_START),  QDate::currentDate());
    _model->setData(_model->index(lastRow, COL_SERVICE_END),    QDate::currentDate());

    for (int col = COL_BILLED; col <= COL_HSA_CLAIMED; ++col)
        _model->setData(_model->index(lastRow, col), 0.0);

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
