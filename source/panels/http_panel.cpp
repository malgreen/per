#include "http_panel.h"

wxBEGIN_EVENT_TABLE(HttpPanel, wxPanel)
    wxEND_EVENT_TABLE();

HttpPanel::HttpPanel(wxWindow *parent, HttpRequestModel *httpRequestModel) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *textLabel = new wxStaticText(this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0);
    verticalSizer->Add(textLabel, 0, wxALL, 5); // TODO: maybe wxEXPAND
}