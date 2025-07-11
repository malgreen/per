#include "http_tab.h"
#include "ui_http_tab.h"

#include <iostream> // TODO: not used?

namespace Per
{
HttpTab::HttpTab(QWidget *parent, HttpRequestModel *httpRequestModel) : QWidget(parent), ui(new Ui::HttpTab)
{
    ui->setupUi(this);
    m_httpRequestModel = httpRequestModel;

    this->InitParametersTable(httpRequestModel);

    connect(ui->parametersTable, &QTableWidget::cellChanged, this, &HttpTab::OnTableCellChanged);
}

HttpTab::~HttpTab()
{
    delete ui;
}

void HttpTab::InitParametersTable(HttpRequestModel *httpRequestModel) const
{
    const auto table = ui->parametersTable;

    // header setup
    table->setColumnWidth(2, 30);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

    // append an empty row
    m_httpRequestModel->parameters.append({.enabled = true, .key = "", .value = ""});

    // add rows to ui
    int i = 0;
    for (auto &item : httpRequestModel->parameters)
    {
        AddTableRow(table, &item);
        i++;
    }

}

void HttpTab::OnTableCellChanged(int row, int column) const
{
    const auto sender = qobject_cast<QTableWidget *>(QObject::sender());
    // TODO: can below be an inline thing?
    QList<EnabledKeyValuePair> &tableData = sender->objectName() == "parametersTable" ? m_httpRequestModel->parameters : m_httpRequestModel->headers;

    if (column == 0)
    {
        tableData[row].enabled = sender->item(row, column)->checkState() == Qt::Checked;
        tableData[row].key = sender->item(row, column)->text();
    } else if (column == 1)
    {
        tableData[row].value = sender->item(row, column)->text();
    }

    AddTableRowIfLastRowNotEmpty(sender);
}

void HttpTab::AddTableRow(QTableWidget *table, EnabledKeyValuePair *rowData) const
{
    const int insertIndex = table->rowCount();
    auto tableName = table->objectName();
    QList<EnabledKeyValuePair> &tableData = table->objectName() == "parametersTable" ? m_httpRequestModel->parameters : m_httpRequestModel->headers;

    table->insertRow(insertIndex);

    // the persistent index is kept track of internally in the QTableWidget,
    // so it is updated whenever the table is updated
    const auto persistentIndex = QPersistentModelIndex(table->model()->index(insertIndex, 2));

    const auto keyTextItem = new QTableWidgetItem(rowData->key);
    keyTextItem->setCheckState(rowData->enabled ? Qt::Checked : Qt::Unchecked);

    const auto valueTextItem = new QTableWidgetItem(rowData->value);

    const auto deleteRowButton = new QPushButton();
    deleteRowButton->setIcon(QIcon(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton)));
    connect(deleteRowButton, &QPushButton::clicked, [this, persistentIndex, table, &tableData, rowData] {
        const auto index = persistentIndex.row();
        table->removeRow(index);
        tableData.removeAt(index);
        AddTableRowIfLastRowNotEmpty(table);
    });

    table->setItem(insertIndex, 0, keyTextItem);
    table->setItem(insertIndex, 1, valueTextItem);
    table->setCellWidget(insertIndex, 2, deleteRowButton);
}

void HttpTab::AddTableRowIfLastRowNotEmpty(QTableWidget *table) const
{
    const int rowCount = table->rowCount();

    if (rowCount < 1 || !IsTableRowEmpty(table, rowCount - 1))
    {
        m_httpRequestModel->parameters.append({.enabled = true, .key = "", .value = ""});
        AddTableRow(table, &m_httpRequestModel->parameters[m_httpRequestModel->parameters.count() - 1]);
    }
}

bool HttpTab::IsTableRowEmpty(const QTableWidget *table, const int row)
{
    for (int i = 0; i < table->columnCount(); i++)
    {
        const auto cell = table->item(row, i);
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
