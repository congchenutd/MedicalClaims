#include "DAO.h"
#include "PageDefault.h"
#include <QDragEnterEvent>
#include <QMessageBox>
#include <QSqlError>

PageDefault::PageDefault(QWidget* parent) :
    Page(parent)
{
    ui.setupUi(this);
    ui.widgetAttachments->hide();
}

void PageDefault::initModel(QSqlTableModel* model)
{
    _model = model;
    _model->select();

    ui.tableView->setModel(_model);
    ui.tableView->hideColumn(COL_ID);
    ui.tableView->resizeColumnsToContents();

    ui.splitter->setSizes(QList<int>() << 500 << 100);

    connect(ui.tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &PageDefault::onSelectionChanged);
}

void PageDefault::add()
{
    int lastRow = _model->rowCount();
    _model->insertRow(lastRow);
    initRow(lastRow);   // default data

    ui.tableView->edit(_model->index(lastRow, COL_ID + 1)); // trigger editting
}

void PageDefault::del()
{
    if (QMessageBox::warning(this, tr("Warning"), tr("Are you sure to delete the selected records?"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        QSet<int> rows;
        foreach (auto idx, ui.tableView->selectionModel()->selectedIndexes())
            rows << idx.row();

        foreach (auto row, rows)
            _model->removeRow(row);
        _model->select();
    }
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

void PageDefault::exportData(const QString& fileName) {
    Q_UNUSED(fileName)
}

void PageDefault::duplicate()
{
    QSet<int> rows;
    foreach (auto idx, ui.tableView->selectionModel()->selectedIndexes())
        rows << idx.row();

    foreach (auto row, rows)
    {
        int lastRow = _model->rowCount();
        _model->insertRow(lastRow);
        initRow(lastRow);
        copyRow(row, lastRow);
    }
    _model->submit();
}

void PageDefault::initRow(int row) {
    _model->setData(_model->index(row, COL_ID), DAO::getNextID(_model->tableName()));
}

void PageDefault::copyRow(int sourceRow, int destinationRow)
{
    for (int col = COL_ID + 1; col < _model->columnCount(); ++col)
        _model->setData(_model->index(destinationRow, col), _model->data(_model->index(sourceRow, col)));
}

void PageDefault::onSelectionChanged(const QItemSelection& selected)
{
    _currentRow = selected.isEmpty() ? -1 : selected.indexes().front().row();
    emit currentRowValid(_currentRow > -1);
}
