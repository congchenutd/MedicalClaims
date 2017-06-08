#include "DAO.h"
#include "PageDefault.h"
#include <QDragEnterEvent>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

PageDefault::PageDefault(QWidget* parent) :
    Page(parent)
{
    ui.setupUi(this);
    ui.widgetAttachments->hide();
}

void PageDefault::setModel(QSqlTableModel* model)
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
        foreach (auto row, getSelectedRows())
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
    foreach (auto row, getSelectedRows())
    {
        int lastRow = _model->rowCount();
        _model->insertRow(lastRow);
        initRow(lastRow);
        copyRow(row, lastRow);
    }
    _model->submit();
}

void PageDefault::autoFill() {}

void PageDefault::setShowFilter(bool) {}

double PageDefault::sumUp() const
{
    double result = 0;
    foreach (auto index, getSelectedIndexes())
        result += _model->data(index).toDouble();
    return result;
}

void PageDefault::initRow(int row) {
    _model->setData(_model->index(row, COL_ID), DAO::getNextID(_model->tableName()));
}

void PageDefault::copyRow(int sourceRow, int destinationRow)
{
    for (int col = COL_ID + 1; col < _model->columnCount(); ++col)
        _model->setData(_model->index(destinationRow, col), _model->data(_model->index(sourceRow, col)));
}

QList<int> PageDefault::getSelectedRows() const
{
    QSet<int> rows;
    foreach (auto idx, ui.tableView->selectionModel()->selectedIndexes())
        rows << idx.row();
    QList<int> rowList = rows.toList();
    std::sort(rowList.begin(), rowList.end());
    return rowList;
}

QModelIndexList PageDefault::getSelectedIndexes() const
{
    QModelIndexList result = ui.tableView->selectionModel()->selectedIndexes();
    std::sort(result.begin(), result.end());
    return result;
}

void PageDefault::onSelectionChanged()
{
    auto selected = getSelectedIndexes();
    _currentRow = selected.isEmpty() ? -1 : selected.front().row();
    int modelID = selected.isEmpty() ? -1 : _model->data(_model->index(_currentRow, COL_ID)).toInt();
    ui.widgetAttachments->setModelID(modelID);
    emit selectionChanged(selected);
}
