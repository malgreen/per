#include "http_tab.h"
#include "ui_http_tab.h"

namespace Per
{
HttpTab::HttpTab(QWidget *parent, HttpRequestModel &httpRequestModel)
    : QWidget(parent), ui(new Ui::HttpTab), m_networkManager(*new QNetworkAccessManager(this)), m_httpRequestModel(httpRequestModel)
{
    ui->setupUi(this);

    this->ui->splitter->setCollapsible(0, false);
    this->ui->splitter->setSizes({1, 0});

    // setup url bar
    this->ui->urlLineEdit->setText(m_httpRequestModel.url);

    // setup tables
    this->SetupEnabledKeyValueTable(*ui->requestParametersTable, m_httpRequestModel.parameters);
    this->SetupEnabledKeyValueTable(*ui->requestHeadersTable, m_httpRequestModel.headers);
    this->ui->responseHeadersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->responseHeadersTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);


    // setup request body tab
    connect(ui->requestBodyTextEdit, &QPlainTextEdit::textChanged,[this]() {
        m_httpRequestModel.bodyContent = ui->requestBodyTextEdit->toPlainText();
    });

    // setup network manager TODO: still not sure if it should be a reference or a pointer
    connect(&m_networkManager, &QNetworkAccessManager::finished, [this](QNetworkReply *reply) {
        if (reply->error())
        {
            // we still want to show it
            qDebug() << reply->errorString();
        }

        m_httpResponseModel = QtReplyToHttpResponse(*reply); // not sure if this works
        reply->deleteLater();
        ui->responseBodyTextEdit->setPlainText(m_httpResponseModel->contentBody);
        // ui->responseHeadersTable->setRowCount();
        for (const auto &header : m_httpResponseModel->headers)
        {
            const int index = ui->responseHeadersTable->rowCount();
            ui->responseHeadersTable->insertRow(index);
            ui->responseHeadersTable->setItem(index - 1, 0, new QTableWidgetItem(header.first));
            ui->responseHeadersTable->setItem(index - 1, 1, new QTableWidgetItem(header.second));
        }

        this->ui->splitter->setSizes({1, 1});
        ui->sendButton->setDisabled(false);
    });

    // setup send button
    connect(ui->sendButton, &QPushButton::clicked, [this]() {
        switch (m_httpRequestModel.method)
        {
        case GET:
            ui->sendButton->setDisabled(true); // a progress indicator would be nice
            m_networkManager.get(HttpRequestToQtRequest(m_httpRequestModel));
            break;
        case POST:
            throw;
        case PUT:
            throw;
        case PATCH:
            throw;
        case DELETE:
            throw;
        case HEAD:
            throw;
        case OPTIONS:
            throw;
        }
    });

}

HttpTab::~HttpTab()
{
    delete ui;
    delete &m_httpRequestModel;
    delete m_httpResponseModel;
}

void HttpTab::SetupEnabledKeyValueTable(QTableWidget &table, QList<EnabledKeyValuePair> &tableData) const
{
    /* one day it would be nice to use a QTableView with a backing model,
     * the current setup means lots of state duplication. which is awesome */

    // ui setup
    table.setColumnWidth(2, 50);
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
    deleteRowButton->setFlat(true);
    deleteRowButton->setIcon(QIcon(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton)));

    connect(deleteRowButton, &QPushButton::clicked, [this, persistentIndex, &table, &tableData] {
        // open confirm dialog?
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
