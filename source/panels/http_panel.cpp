#include "http_panel.h"

wxBEGIN_EVENT_TABLE(HttpPanel, wxPanel)
    wxEND_EVENT_TABLE();

HttpPanel::HttpPanel(wxWindow *parent, HttpRequestModel *httpRequestModel) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    int border = 5;
    wxBoxSizer *rootSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *textLabel = new wxStaticText(this, wxID_ANY, httpRequestModel->url, wxDefaultPosition, wxDefaultSize, 0);
    rootSizer->Add(textLabel, 0, wxALL | wxEXPAND, border);

    /* HTTP METHOD + URL SECTION */
    wxBoxSizer *urlSizer = new wxBoxSizer(wxHORIZONTAL);

    urlSizer->Add(BuildHttpMethodComboBox(), 0, wxALL, border); // guess 5 is the border size

    wxTextCtrl *urlTextCtrl = new wxTextCtrl(this, wxID_ANY, httpRequestModel->url, wxDefaultPosition, wxDefaultSize, 0);
    urlSizer->Add(urlTextCtrl, 1, wxALL, border);

    wxButton *sendButton = new wxButton(this, wxID_ANY, "Send", wxDefaultPosition, wxDefaultSize, 0); // apparently, we don't need default stuff
    urlSizer->Add(sendButton, 0, wxALL, border);

    /* PARAMS, HEADERS, BODY, AUTH SECTION */

    /* RESPONSE SECTION */

    /* FINAL COMP */
    rootSizer->Add(urlSizer, 1, wxEXPAND | wxALL, border);

    this->SetSizerAndFit(rootSizer);
}

wxComboBox *HttpPanel::BuildHttpMethodComboBox()
{
    wxComboBox *comboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, nullptr, wxCB_DROPDOWN | wxCB_READONLY);
    for (auto &method : HttpMethodMap)
    {
        comboBox->Append(method.second);
    }
    comboBox->SetSelection(0); // future: should be loaded from state
    return comboBox;
}
