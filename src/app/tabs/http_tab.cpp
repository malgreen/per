#include "http_tab.h"
#include "ui_http_tab.h"

#include "../../util/signal_blocker.h"

namespace Per
{
HttpTab::HttpTab(QWidget *parent, HttpRequestModel_t &httpRequestModel): QWidget(parent),
    ui(new Ui::HttpTab),
    m_networkManager(*new QNetworkAccessManager(this)),
    m_httpRequestModel(httpRequestModel)
{
    ui->setupUi(this);

    this->ui->splitter->setCollapsible(0, false);
    this->ui->splitter->setSizes({1, 0});

    this->SetupUrlGroup();
    this->SetupRequestGroup();
    this->SetupResponseGroup();

    // setup network manager
    connect(&m_networkManager, &QNetworkAccessManager::finished, this, &HttpTab::OnNetworkManagerFinished);
}

HttpTab::~HttpTab()
{
    delete ui;
    delete &m_httpRequestModel;
    delete m_httpResponseModel;
}

void HttpTab::SetupUrlGroup()
{
    // method combobox

    // url text field
    this->ui->urlLineEdit->setText(m_httpRequestModel.url);
    connect(ui->urlLineEdit, &QLineEdit::textChanged, [this](const QString &text) { // &HttpTab::OnUrlChanged
        auto url = QUrl(text);
        m_httpRequestModel.url = url.toString(QUrl::RemoveQuery);
        GetParametersFromUrl(url);
    });

    // send button
    connect(ui->sendButton, &QPushButton::clicked, this, &HttpTab::OnSendButtonClicked);
}
void HttpTab::SetupRequestGroup()
{
    // params / headers tables
    this->SetupEnabledKeyValueTable(*ui->requestParametersTable, m_httpRequestModel.parameters);
    this->SetupEnabledKeyValueTable(*ui->requestHeadersTable, m_httpRequestModel.headers);

    // handle static headers
    for (int i = 0; i < ui->requestHeadersTable->rowCount(); ++i)
    {
        const auto keyCell = ui->requestHeadersTable->item(i, 0);
        if (!StaticHttpHeaderKeys.contains(keyCell->text()))
            continue;
        keyCell->setBackground(QBrush(Qt::gray));
        keyCell->setFlags(Qt::ItemIsEnabled);

        const auto valueCell = ui->requestHeadersTable->item(i, 1);
        valueCell->setText("<set during send>");
        valueCell->setBackground(QBrush(Qt::gray));
        valueCell->setFlags(Qt::ItemIsEnabled);

        ui->requestHeadersTable->removeCellWidget(i, 2);
        const auto deleteBtnCell = new QTableWidgetItem();
        deleteBtnCell->setBackground(QBrush(Qt::gray));
        deleteBtnCell->setFlags(Qt::ItemIsEnabled);
        ui->requestHeadersTable->setItem(i, 2, deleteBtnCell);
    }


    // request body editor
    connect(ui->requestBodyTextEdit, &QPlainTextEdit::textChanged, [this]() {
        m_httpRequestModel.bodyContent = ui->requestBodyTextEdit->toPlainText();
    });
}

void HttpTab::SetupResponseGroup()
{
    // headers table TODO: fix column sizing
    this->ui->responseHeadersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->responseHeadersTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    this->ui->responseHeadersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // response body
    this->ui->responseBodyTextEdit->setReadOnly(true);

}

void HttpTab::SetupEnabledKeyValueTable(QTableWidget &table, QList<EnabledKeyValuePair_t> &tableData) const
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
        qDebug() << "cellChanged called" << row << column;
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
        GetUrlFromParameters();
    });
}

void HttpTab::OnNetworkManagerFinished(QNetworkReply *reply)
{
    // TODO: have some kind of status code text (that also changes color)
    if (reply->error())
    {
        qDebug() << reply->errorString();
    }

    m_httpResponseModel = QtReplyToHttpResponse(*reply); // not sure if this works
    reply->deleteLater();
    ui->responseBodyTextEdit->setPlainText(m_httpResponseModel->contentBody);
    ui->responseHeadersTable->setRowCount(0);
    for (const auto &header : m_httpResponseModel->headers)
    {
        const int index = ui->responseHeadersTable->rowCount();
        ui->responseHeadersTable->insertRow(index);
        ui->responseHeadersTable->setItem(index - 1, 0, new QTableWidgetItem(header.first));
        ui->responseHeadersTable->setItem(index - 1, 1, new QTableWidgetItem(header.second));
    }

    this->ui->splitter->setSizes({1, 1});
    ui->sendButton->setDisabled(false);
}

void HttpTab::OnSendButtonClicked()
{
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
}
void HttpTab::GetParametersFromUrl(const QUrl &url) const
{
    // TODO: rewrite to make it more intuitive
    // the idea in this function is that we re-use enabled parameters by overwriting values (preserving the order)

    auto &modelParams = this->m_httpRequestModel.parameters; // alias looks better

    // remove empty pair at the end of params list (if it is enabled)
    if (modelParams.last().enabled && modelParams.last().key.isEmpty() && modelParams.last().value.isEmpty())
    {
        modelParams.pop_back();
    }

    // change existing model parameters to the ones in the url bar
    auto paramStrings = url.query().split('&');
    int i = -1; // because of the weird while loop
    for (const auto &paramString : paramStrings)
    {
        // define the key/value parts
        auto split = paramString.split('=');
        auto key = split.at(0); // pretty sure we always get 1 item from a split
        auto value = split.count() > 1 ? split.at(1) : QString();

        // iterate over model's existing parameters and mutate one accordingly
        auto done = false; // instead of break inside the loop
        while (i < modelParams.count() && !done)
        {
            i++;
            if (i >= modelParams.count()) // add a new parameter if necessary
            {
                modelParams.append({.enabled = true, .key = key, .value = value});
                done = true;
            }
            else if (modelParams.at(i).enabled == true) // or mutate existing param
            {
                modelParams[i].key = key; // TODO: afaik this can't be done nicely with .at(i)
                modelParams[i].value = value;
                done = true;
            }
        }
    }

    // find out if we deleted a parameter from the url
    while (std::ranges::count_if(modelParams, [](const auto &item) { return item.enabled; }) > paramStrings.count())
    {
        for (int j = modelParams.count() - 1; j >= 0; j--)
        {
            if (modelParams.at(j).enabled == true)
            {
                modelParams.removeAt(j);
                break;
            }
        }
    }

    // update the visual table
    ui->requestParametersTable->setRowCount(0);
    for (auto &param : modelParams)
    {
        AddTableRow(*ui->requestParametersTable, modelParams, param);
    }
    AddTableRowIfLastRowNotEmpty(*ui->requestParametersTable, modelParams);
}
void HttpTab::GetUrlFromParameters() const
{
    // 1. get the existing url from model (we could remove query again to make sure)
    auto url = QUrl(m_httpRequestModel.url);

    // 2. for each enabled parameter in the model, add it to the url
    auto query = QUrlQuery();
    for (const auto &param : m_httpRequestModel.parameters)
    {
        if (param.enabled && !(param.key.isEmpty() && param.value.isEmpty()))
        {
            query.addQueryItem(param.key, param.value);
        }
    }
    url.setQuery(query);

    // 3.
    auto blocker = SignalBlocker(*ui->urlLineEdit);
    ui->urlLineEdit->setText(url.toString());

    // url.setQuery()
}

void HttpTab::AddTableRow(QTableWidget &table, QList<EnabledKeyValuePair_t> &tableData, EnabledKeyValuePair_t &rowData) const
{
    // we don't want to constantly trigger cellChanged (early returns means this breaks)
    // it would be nicer to have a class with a destructor that disables the signal blocking
    auto blocker = SignalBlocker(table);

    const int insertIndex = table.rowCount();

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

void HttpTab::AddTableRowIfLastRowNotEmpty(QTableWidget &table, QList<EnabledKeyValuePair_t> &tableData) const
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
