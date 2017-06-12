#include "DAO.h"
#include "PageDefault.h"
#include "MyModel.h"
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

void PageDefault::setModel(MyModel* model)
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
    _model->submit();

    ui.tableView->edit(_model->index(lastRow, COL_ID + 1)); // trigger editting
    ui.tableView->scrollToBottom();
}

void PageDefault::del()
{
    if (QMessageBox::warning(this, tr("Warning"), tr("Are you sure to delete the selected records?"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        foreach (auto row, getSelectedRows())
            _model->removeRow(row);
        refresh();
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
    QList<int> ids;
    foreach (auto row, getSelectedRows())
    {
        int lastRow = _model->rowCount();
        _model->insertRow(lastRow);
        initRow(lastRow);
        copyRow(row, lastRow);
        ids << _model->data(_model->index(lastRow, COL_ID)).toInt();
        _model->submit();
    }

    // sort and highlight duplicated rows
    keepSorted();
    ui.tableView->selectionModel()->clear();
    foreach (auto id, ids)
    {
        auto matches = _model->match(_model->index(0, COL_ID), Qt::DisplayRole, id, 1);
        if (!matches.isEmpty())
        {
            auto idx = matches.front();
            ui.tableView->selectionModel()->select(idx,
                                                   QItemSelectionModel::Select | QItemSelectionModel::Rows);
            ui.tableView->scrollTo(idx);
        }
    }
}

void PageDefault::autoFill() {
    _model->autoFill(getSelectedIndexes());
}

void PageDefault::setShowFilter(bool) {}

double PageDefault::sumUpSelected() const
{
    double result = 0;
    foreach (auto index, getSelectedIndexes())
        result += _model->data(index).toDouble();
    return result;
}

void PageDefault::clearCell()
{
    foreach (auto index, getSelectedIndexes())
        if (index.column() > COL_ID)
        {
            _model->resetCell(index);
            _model->submit();
        }
}

void PageDefault::initRow(int row) {
    _model->initRow(row);
}

void PageDefault::copyRow(int sourceRow, int destinationRow) {
    _model->copyRow(sourceRow, destinationRow);
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

void PageDefault::keepSorted() {
    ui.tableView->sortByColumn(ui.tableView->horizontalHeader()->sortIndicatorSection(),
                               ui.tableView->horizontalHeader()->sortIndicatorOrder());
}

void PageDefault::onSelectionChanged()
{
    auto selected = getSelectedIndexes();
    _currentRow     = selected.isEmpty() ? -1 : selected.front().row();
    int recordID    = selected.isEmpty() ? -1 : _model->data(_model->index(_currentRow, COL_ID)).toInt();
    ui.widgetAttachments->setRecordID(recordID);
    emit selectionChanged(selected);
}
