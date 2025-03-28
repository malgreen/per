#include "wx/button.h"
#include "wx/sizer.h"
#include <wx/aui/auibook.h>
#include <cstddef>
#include "main_frame.h"
#include "../panels/http_panel.h"
#include "../utilities/per_ids.h"
#include "../utilities/about.h"
#include "../utilities/per_aui_tab_art.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
        EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
            EVT_MENU(perID_NEW_TAB, MainFrame::OnNewTab)
                wxEND_EVENT_TABLE();

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    this->BuildMenuBar();

    wxBoxSizer *rootSizer = new wxBoxSizer(wxHORIZONTAL);

    m_TabNotebook = new wxAuiNotebook(this, wxID_ANY);
    m_TabNotebook->SetArtProvider(new PerAuiTabArt());

    rootSizer->Add(m_TabNotebook, 1, wxEXPAND | wxALL);

    this->SetSizer(rootSizer);
}

void MainFrame::BuildMenuBar()
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(perID_NEW_TAB, "&New tab");

    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

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
    wxString title = PROJECT_NAME;
    wxString content = "version " + PROJECT_VERSION + "\n" + "commit " + PROJECT_GIT_HASH + "\n" + "built " + PROJECT_BUILD_DATE;
    wxMessageBox(content, title, wxOK | wxICON_INFORMATION);
}

void MainFrame::OnNewTab(wxCommandEvent &event)
{
    HttpRequestModel httpRequestModel1 = {
        .method = HttpMethod::GET,
        .url = "https://www.google.com"};

    HttpPanel *httpPanel1 = new HttpPanel(m_TabNotebook, &httpRequestModel1);
    m_TabNotebook->AddPage(httpPanel1, httpRequestModel1.url, true);
}
