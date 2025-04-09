#include "http_panel.h"

wxBEGIN_EVENT_TABLE(HttpPanel, wxPanel)
    EVT_SIZE(HttpPanel::OnSize)
        EVT_GRID_CELL_CHANGED(HttpPanel::OnGridCellChange)
            wxEND_EVENT_TABLE();

// TODO: we use 'this' as parent parameter throughout this file, but maybe that is wrong? we should just more specific parents?
HttpPanel::HttpPanel(wxWindow *parent, HttpRequestModel *httpRequestModel) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    m_HttpRequestModel = httpRequestModel;
    constexpr int border = 5;
    const auto rootSizer = new wxBoxSizer(wxVERTICAL);

    const auto textLabel = new wxStaticText(this, wxID_ANY, httpRequestModel->url, wxDefaultPosition, wxDefaultSize, 0);
    rootSizer->Add(textLabel, 0, wxALL | wxEXPAND, border);

    /* HTTP METHOD + URL SECTION */
    const auto urlSizer = new wxBoxSizer(wxHORIZONTAL);

    m_HttpMethodComboBox = BuildHttpMethodComboBox();
    urlSizer->Add(m_HttpMethodComboBox, 0, wxALL, border); // guess 5 is the border size

    const auto urlTextCtrl = new wxTextCtrl(this, wxID_ANY, httpRequestModel->url, wxDefaultPosition, wxDefaultSize, 0);
    urlSizer->Add(urlTextCtrl, 1, wxALL, border);

    const auto sendButton = new wxButton(this, wxID_ANY, "Send", wxDefaultPosition, wxDefaultSize, 0); // TODO: apparently we don't need default stuff?
    urlSizer->Add(sendButton, 0, wxALL, border);

    /* PARAMS, HEADERS, BODY, AUTH SECTION */
    auto *notebook = new wxNotebook(this, wxID_ANY);

    auto *paramsPanel = BuildParamsPanel(notebook);
    notebook->AddPage(paramsPanel, "Parameters");

    auto *headersPanel = BuildHeadersPanel(notebook);
    notebook->AddPage(headersPanel, "Headers");

    /* RESPONSE SECTION */

    /* FINAL COMP */
    rootSizer->Add(urlSizer, 1, wxEXPAND | wxALL, border);
    rootSizer->Add(notebook, 1, wxEXPAND | wxALL, border);

    this->SetSizerAndFit(rootSizer);
    // this->AutoFitGrids();
}

wxComboBox *HttpPanel::BuildHttpMethodComboBox()
{
    auto *comboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, nullptr, wxCB_DROPDOWN | wxCB_READONLY);
    for (auto &method : HttpMethodMap)
    {
        comboBox->Append(method.second);
    }
    comboBox->SetSelection(0); // future: should be loaded from state
    return comboBox;
}

wxPanel *HttpPanel::BuildParamsPanel(wxWindow *parent)
{
    auto *panel = new wxPanel(parent, wxID_ANY);
    auto *sizer = new wxBoxSizer(wxVERTICAL);

    m_ParamsGrid = new wxGrid(panel, perID_HTTP_PARAMS_GRID);
    m_ParamsGrid->CreateGrid(1, 2);
    m_ParamsGrid->SetColLabelValue(0, "Key");
    m_ParamsGrid->SetColLabelValue(1, "Value");
    m_ParamsGrid->AutoSizeColumns();
    m_ParamsGrid->DisableDragGridSize();
    m_ParamsGrid->DisableDragColLabelSize();
    m_ParamsGrid->DisableDragColSize();
    m_ParamsGrid->DisableDragRowLabelSize();
    m_ParamsGrid->DisableDragRowSize();

    for (auto i = m_HttpRequestModel->parameters.begin(); i != m_HttpRequestModel->parameters.end(); ++i)
    {
        m_ParamsGrid->AppendRows(1);
        int row = m_ParamsGrid->GetNumberRows() - 1;
        m_ParamsGrid->SetCellValue(row, 0, i->first);
        m_ParamsGrid->SetCellValue(row, 1, i->second);
    }

    sizer->Add(m_ParamsGrid, 1, wxEXPAND | wxALL);

    panel->SetSizer(sizer);
    return panel;
}

wxPanel *HttpPanel::BuildHeadersPanel(wxWindow *parent)
{
    auto *panel = new wxPanel(parent, wxID_ANY);
    auto *sizer = new wxBoxSizer(wxVERTICAL);

    m_HeadersGrid = new wxGrid(panel, perID_HTTP_HEADERS_GRID);
    m_HeadersGrid->CreateGrid(1, 2);
    m_HeadersGrid->SetColLabelValue(0, "Key");
    m_HeadersGrid->SetColLabelValue(1, "Value");
    m_HeadersGrid->AutoSizeColumns();
    m_HeadersGrid->DisableDragGridSize();
    m_HeadersGrid->DisableDragColLabelSize();
    m_HeadersGrid->DisableDragColSize();
    m_HeadersGrid->DisableDragRowLabelSize();
    m_HeadersGrid->DisableDragRowSize();

    for (auto i = m_HttpRequestModel->headers.begin(); i != m_HttpRequestModel->headers.end(); ++i)
    {
        m_HeadersGrid->AppendRows(1);
        int row = m_HeadersGrid->GetNumberRows() - 1;
        m_HeadersGrid->SetCellValue(row, 0, i->first);
        m_HeadersGrid->SetCellValue(row, 1, i->second);
    }

    sizer->Add(m_HeadersGrid, 1, wxEXPAND | wxALL);

    panel->SetSizer(sizer);
    return panel;
}

wxPanel *HttpPanel::BuildBodyPanel(wxWindow *parent)
{
    return nullptr;
}

void HttpPanel::OnGridCellChange(wxGridEvent &event)
{
    wxGrid *grid = nullptr;

    switch (event.GetId())
    {
    case perID_HTTP_PARAMS_GRID:
        grid = m_ParamsGrid;
        break;
    case perID_HTTP_HEADERS_GRID:
        grid = m_HeadersGrid;
        break;
    default:
        break;
    }

    if (grid == nullptr)
        return;

    if (const auto changedRowIndex = event.GetRow(); grid->GetCellValue(changedRowIndex, 0).IsEmpty() && grid->GetCellValue(changedRowIndex, 1).IsEmpty())
    {
        grid->DeleteRows(changedRowIndex);
    }

    if (const auto lastRowIndex = grid->GetNumberRows() - 1; !grid->GetCellValue(lastRowIndex, 0).IsEmpty() || !grid->GetCellValue(lastRowIndex, 1).IsEmpty())
    {
        grid->AppendRows(1);
    }
}

void HttpPanel::OnSize(wxSizeEvent &event)
{
    // TODO: a couple of limitations with this approach:
    // 1. it doesn't call OnSize when initially rendered, meaning this doesn't get called, meaning the grids are ugly
    // 2. for some reason, it does not get called when going fullscreen
    this->AutoFitGrids();
    event.Skip();
}

void HttpPanel::AutoFitGrids()
{
    // resize the grids
    wxGrid *grids[2] = {
        m_ParamsGrid,
        m_HeadersGrid,
    };

    for (auto grid : grids)
    {
        // if (!grid->IsDrawn???) continue; TODO
        int totalWidth = grid->GetSize().GetWidth(); // TODO: also take header column into account
        int numCols = grid->GetNumberCols();
        int colWidth = totalWidth / numCols;

        for (int col = 0; col < numCols; ++col)
        {
            grid->SetColSize(col, colWidth);
        }
    }
}
