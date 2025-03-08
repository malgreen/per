#include "http_panel.h"

wxBEGIN_EVENT_TABLE(HttpPanel, wxPanel)
    wxEND_EVENT_TABLE();

HttpPanel::HttpPanel(wxWindow *parent, HttpRequestModel *httpRequestModel) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    int border = 5;
    wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *textLabel = new wxStaticText(this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0);
    verticalSizer->Add(textLabel, 0, wxALL, border); // TODO: maybe wxEXPAND

    // URL SECTION
    wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    wxComboBox *comboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
    horizontalSizer->Add(comboBox, 0, wxALL, border); // guess 5 is the border size

    wxTextCtrl *urlTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
    horizontalSizer->Add(urlTextCtrl, 0, wxALL, border);

    wxButton *sendButton = new wxButton(this, wxID_ANY, "Send", wxDefaultPosition, wxDefaultSize, 0); // apparently, we don't need default stuff
    horizontalSizer->Add(sendButton, 0, wxALL, border);
}