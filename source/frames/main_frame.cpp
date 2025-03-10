#include "wx/button.h"
#include "wx/sizer.h"
#include <wx/aui/auibook.h>
#include <cstddef>
#include "main_frame.h"
#include "../panels/http_panel.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
        EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
            EVT_MENU(perID_NEW_TAB, MainFrame::OnNewTab)
                wxEND_EVENT_TABLE();

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    this->BuildMenuBar();

    wxBoxSizer *rootSizer = new wxBoxSizer(wxHORIZONTAL);

    m_TabNotebook = new wxAuiNotebook(this, wxID_ANY);

    rootSizer->Add(m_TabNotebook, 1, wxEXPAND | wxALL);

    this->SetSizer(rootSizer);
}

void MainFrame::BuildMenuBar()
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(perID_NEW_TAB, "&new tab");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT); // TODO: should be in about menu?

    wxMenuBar *menuBar = new wxMenuBar;

    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
}

void MainFrame::OnExit(wxCommandEvent &event)
{
    // TODO: remember window state on exit (maybe in some .json somewhere), use it when starting the application again
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
    // TODO: generate with about.h.in
    wxString title = "About Hello World";
    wxString content = "This is a wxWidgets' Hello world sample";
    wxMessageBox(content, title, wxOK | wxICON_INFORMATION); // TODO: do we need icon?
}

void MainFrame::OnNewTab(wxCommandEvent &event)
{
    HttpRequestModel httpRequestModel1 = {
        .method = HttpMethod::GET,
        .url = "https://www.google.com"};

    HttpPanel *httpPanel1 = new HttpPanel(m_TabNotebook, &httpRequestModel1);
    m_TabNotebook->AddPage(httpPanel1, httpRequestModel1.url);
}
