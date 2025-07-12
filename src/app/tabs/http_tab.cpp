#include "http_tab.h"
#include "ui_http_tab.h"

#include <iostream> // TODO: not used?

namespace Per
{
HttpTab::HttpTab(QWidget *parent, HttpRequestModel &httpRequestModel)
    : QWidget(parent), ui(new Ui::HttpTab), m_httpRequestModel(httpRequestModel)
{
    ui->setupUi(this);

    this->SetupEnabledKeyValueTable(*ui->parametersTable, m_httpRequestModel.parameters);
    this->SetupEnabledKeyValueTable(*ui->headersTable, m_httpRequestModel.headers);
}

HttpTab::~HttpTab()
{
    delete ui;
    delete &m_httpRequestModel;
}

void HttpTab::SetupEnabledKeyValueTable(QTableWidget &table, QList<EnabledKeyValuePair> &tableData) const
{
    /* one day it would be nice to use a QTableView with a backing model,
     * the current setup means lots of state duplication. which is awesome */

    // ui setup
    table.setColumnWidth(2, 30);
    table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table.horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

    // add empty list entry
    tableData.append({.enabled = true, .key = "", .value = ""});

    // add rows to ui
    int i = 0;
    for (auto &item : tableData)
    {
        AddTableRow(table, tableData, item);
        i++;
    }

    // signal setup
    connect(&table, &QTableWidget::cellChanged, [this, &table, &tableData](const int row, const int column) {
        if (column == 0)
        {
            tableData[row].enabled = table.item(row, column)->checkState() == Qt::Checked;
            tableData[row].key = table.item(row, column)->text();
        }
        else if (column == 1)
        {
            tableData[row].value = table.item(row, column)->text();
        }
        AddTableRowIfLastRowNotEmpty(table, tableData);
    });
}

void HttpTab::AddTableRow(QTableWidget &table, QList<EnabledKeyValuePair> &tableData, EnabledKeyValuePair &rowData) const
{
    const int insertIndex = table.rowCount();
    auto tableName = table.objectName();

    table.insertRow(insertIndex);

    // the persistent index is kept track of internally in the QTableWidget,
    // so it is updated whenever the table is updated
    const auto persistentIndex = QPersistentModelIndex(table.model()->index(insertIndex, 2));

    const auto keyTextItem = new QTableWidgetItem(rowData.key);
    keyTextItem->setCheckState(rowData.enabled ? Qt::Checked : Qt::Unchecked);

    const auto valueTextItem = new QTableWidgetItem(rowData.value);

    const auto deleteRowButton = new QPushButton();
    deleteRowButton->setIcon(QIcon(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton)));

    connect(deleteRowButton, &QPushButton::clicked, [this, persistentIndex, &table, &tableData] {
        const auto index = persistentIndex.row();
        table.removeRow(index);
        tableData.removeAt(index);
        AddTableRowIfLastRowNotEmpty(table, tableData);
    });

    table.setItem(insertIndex, 0, keyTextItem);
    table.setItem(insertIndex, 1, valueTextItem);
    table.setCellWidget(insertIndex, 2, deleteRowButton);
}

void HttpTab::AddTableRowIfLastRowNotEmpty(QTableWidget &table, QList<EnabledKeyValuePair> &tableData) const
{
    const int rowCount = table.rowCount();

    if (rowCount < 1 || !IsTableRowEmpty(table, rowCount - 1))
    {
        tableData.append({.enabled = true, .key = "", .value = ""});
        AddTableRow(table, tableData, tableData[tableData.count() - 1]);
    }
}

bool HttpTab::IsTableRowEmpty(const QTableWidget &table, const int row)
{
    for (int i = 0; i < table.columnCount(); i++)
    {
        const auto cell = table.item(row, i);
        if (cell == nullptr)
            continue;

        auto value = cell->text();
        if (value.isEmpty())
            continue;

        return false; // if we find a cell that has a value
    }
    return true; // if we don't
}

} // namespace Per
