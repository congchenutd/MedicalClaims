#include "DAO.h"
#include "PageDefault.h"
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

PageDefault::PageDefault(QWidget* parent) :
    Page(parent)
{
    ui.setupUi(this);
}

void PageDefault::initModel(QSqlTableModel* model)
{
    _model = model;
    _model->select();
    ui.tableView->setModel(_model);
    ui.tableView->hideColumn(COL_ID);

    connect(ui.tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &PageDefault::onCurrentRowChanged);
}

void PageDefault::add()
{
    int lastRow = _model->rowCount();
    _model->insertRow(lastRow);
    _model->setData(_model->index(lastRow, COL_ID), DAO::getNextID(_model->tableName()));
    ui.tableView->edit(_model->index(lastRow, COL_ID + 1));
}

void PageDefault::del()
{
    QSet<QModelIndex> indexes = ui.tableView->selectionModel()->selectedIndexes().toSet();
    foreach (const QModelIndex& idx, indexes)
        _model->removeRow(idx.row());
    _model->select();
}

void PageDefault::save()
{
    _model->database().transaction();
    if (_model->submitAll())
        _model->database().commit();
    else
    {
        _model->database().rollback();
        QMessageBox::warning(this, tr("Database error"),
                             tr("The database reported an error: %1").arg(_model->lastError().text()));
    }
}

void PageDefault::refresh() {
    _model->select();
}

void PageDefault::onCurrentRowChanged(const QModelIndex& idx)
{
    emit currentRowValid(idx.isValid());
}
