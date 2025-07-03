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
    EVT_MENU(perID_CLOSE_TAB, MainFrame::OnCloseTab)
    EVT_MENU(perID_LAST_TAB, MainFrame::OnSwitchTab)
wxEND_EVENT_TABLE();

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    // SHORTCUT STUFF
    m_TabActionMap = {
        {perID_TAB_1, 0}, {perID_TAB_2, 1},
        {perID_TAB_3, 2}, {perID_TAB_4, 3},
        {perID_TAB_5, 4}, {perID_TAB_6, 5},
        {perID_TAB_7, 6}, {perID_TAB_8, 7},
    };

    constexpr int acceleratorEntriesCount = 11;
    wxAcceleratorEntry acceleratorEntries[acceleratorEntriesCount];
    acceleratorEntries[0].Set(wxACCEL_CTRL, (int) 'T', perID_NEW_TAB);
    acceleratorEntries[1].Set(wxACCEL_CMD, (int) 'W', perID_CLOSE_TAB);
    acceleratorEntries[2].Set(wxACCEL_CMD, (int) '9', perID_LAST_TAB);
    for (int i = 0; i < 8; ++i)
    {
        const auto actionId = perID_TAB_1 + i;
        acceleratorEntries[i].Set(wxACCEL_CMD, static_cast<wxChar>(i+1), actionId);
        Bind(wxEVT_MENU, &MainFrame::OnSwitchTab, this, actionId);
    }

    wxAcceleratorTable acceleratorTable(acceleratorEntriesCount, acceleratorEntries);
    this->wxWindowBase::SetAcceleratorTable(acceleratorTable);

    // CONSTRUCTING THE UI
    const auto rootSizer = new wxBoxSizer(wxHORIZONTAL);

    m_TabNotebook = new wxAuiNotebook(this, wxID_ANY);
    m_TabNotebook->SetArtProvider(new PerAuiTabArt());

    rootSizer->Add(m_TabNotebook, 1, wxEXPAND | wxALL);

    this->SetSizer(rootSizer);

    // menu bar
    this->BuildMenuBar();
}

void MainFrame::BuildMenuBar()
{
    auto *menuFile = new wxMenu;
    menuFile->Append(perID_NEW_TAB, "&New tab");

    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    auto *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    const auto menuBar = new wxMenuBar;

    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
}

void MainFrame::OnExit(wxCommandEvent &event)
{
    // TODO: remember window state on exit (maybe in some .json somewhere), use it when starting the application again
    if (!Close(false))
    {
        Close(true);
    }
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
    const wxString title = PROJECT_NAME;
    const wxString content = "version " + PROJECT_VERSION + "\n" + "commit " + PROJECT_GIT_HASH + "\n" + "built " + PROJECT_BUILD_DATE;
    wxMessageBox(content, title, wxOK | wxICON_INFORMATION);
}

void MainFrame::OnNewTab(wxCommandEvent &event)
{
    CreateTab();
    event.Skip();
}

void MainFrame::OnCloseTab(wxCommandEvent &event)
{
    event.Skip();
}

void MainFrame::OnSwitchTab(wxCommandEvent &event)
{
    const auto eventId = event.GetId();
    const auto pageCount = m_TabNotebook->GetPageCount();
    if (eventId == perID_LAST_TAB)
    {
        m_TabNotebook->SetSelection(pageCount - 1);
    }
    else if (m_TabActionMap.contains(eventId))
    {
        const auto tabNum = m_TabActionMap[eventId];
        if (pageCount > tabNum)
        {
            m_TabNotebook->SetSelection(tabNum);
        }
    }
    event.Skip();
}

void MainFrame::CreateTab()
{
    HttpRequestModel requestModel = {
        .method = HttpMethod::GET,
        .url = "https://www.google.com",
        .parameters = {},
        .headers = {},
        .contentType = "application/json",
        .bodyContent = "",
    };

    SetDefaultHttpHeaders(requestModel);

    auto *httpPanel = new HttpPanel(m_TabNotebook, &requestModel);
    m_TabNotebook->AddPage(httpPanel, requestModel.url, true);
}
