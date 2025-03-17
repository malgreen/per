#include "http_panel.h"

wxBEGIN_EVENT_TABLE(HttpPanel, wxPanel)
    EVT_GRID_CELL_CHANGED(HttpPanel::OnGridCellChange)
        wxEND_EVENT_TABLE();

// TODO: we use 'this' as parent parameter throughout this file, but maybe that is wrong? we should just more specific parents?
HttpPanel::HttpPanel(wxWindow *parent, HttpRequestModel *httpRequestModel) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    int border = 5;
    wxBoxSizer *rootSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *textLabel = new wxStaticText(this, wxID_ANY, httpRequestModel->url, wxDefaultPosition, wxDefaultSize, 0);
    rootSizer->Add(textLabel, 0, wxALL | wxEXPAND, border);

    /* HTTP METHOD + URL SECTION */
    wxBoxSizer *urlSizer = new wxBoxSizer(wxHORIZONTAL);

    m_HttpMethodComboBox = BuildHttpMethodComboBox();
    urlSizer->Add(m_HttpMethodComboBox, 0, wxALL, border); // guess 5 is the border size

    wxTextCtrl *urlTextCtrl = new wxTextCtrl(this, wxID_ANY, httpRequestModel->url, wxDefaultPosition, wxDefaultSize, 0);
    urlSizer->Add(urlTextCtrl, 1, wxALL, border);

    wxButton *sendButton = new wxButton(this, wxID_ANY, "Send", wxDefaultPosition, wxDefaultSize, 0); // TODO: apparently, we don't need default stuff?
    urlSizer->Add(sendButton, 0, wxALL, border);

    /* PARAMS, HEADERS, BODY, AUTH SECTION */
    auto *notebook = new wxNotebook(this, wxID_ANY);

    auto *paramsPanel = BuildParamsPanel(notebook);
    notebook->AddPage(paramsPanel, "Parameters");

    /* RESPONSE SECTION */

    /* FINAL COMP */
    rootSizer->Add(urlSizer, 1, wxEXPAND | wxALL, border);
    rootSizer->Add(notebook, 1, wxEXPAND | wxALL, border);

    this->SetSizerAndFit(rootSizer);
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
    auto *mainSizer = new wxBoxSizer(wxVERTICAL);

    m_ParamsGrid = new wxGrid(panel, perID_HTTP_PARAMS_GRID);
    m_ParamsGrid->CreateGrid(1, 2);
    m_ParamsGrid->SetColLabelValue(0, "Key");
    m_ParamsGrid->SetColLabelValue(1, "Value");

    mainSizer->Add(m_ParamsGrid, wxEXPAND | wxALL);

    panel->SetSizer(mainSizer);
    return panel;
}

wxPanel *HttpPanel::BuildHeadersPanel(wxWindow *parent)
{
    return nullptr;
}

wxPanel *HttpPanel::BuildBodyPanel(wxWindow *parent)
{
    return nullptr;
}

void HttpPanel::OnGridCellChange(wxGridEvent &event)
{
    auto grid = event.GetId() == perID_HTTP_PARAMS_GRID ? m_ParamsGrid : nullptr;

    auto changedRowIndex = event.GetRow();
    if (grid->GetCellValue(changedRowIndex, 0).IsEmpty() && grid->GetCellValue(changedRowIndex, 1).IsEmpty())
    {
        grid->DeleteRows(changedRowIndex);
    }

    auto lastRowIndex = grid->GetNumberRows() - 1;
    if (!grid->GetCellValue(lastRowIndex, 0).IsEmpty() || !grid->GetCellValue(lastRowIndex, 1).IsEmpty())
    {
        grid->AppendRows(1);
    }
}
