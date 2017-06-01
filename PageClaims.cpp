#include "PageClaims.h"
#include "DAO.h"
#include "DateDelegate.h"
#include "DlgAttachment.h"
#include <QDate>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>

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

void PageClaims::onSelectionChanged(const QItemSelection& selected)
{
    int claimID = selected.isEmpty() ? -1 : _model->data(_model->index(_currentRow, COL_ID)).toInt();
    ui.widgetAttachments->setClaimID(claimID);
}
