#include "../per_app.h"
#include "wx/button.h"
#include "wx/sizer.h"
#include <cstddef>
#include "main_frame.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
        EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
            wxEND_EVENT_TABLE()
                wxIMPLEMENT_APP(PerApp);

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);

    wxAnyButton *button = new wxButtonBase();

    boxSizer->Add(button, wxEXPAND); // TODO: crash on startup

    this->SetSizer(boxSizer);
}

void MainFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
    wxString title = "About Hello World";
    wxString content = "This is a wxWidgets' Hello world sample";
    wxMessageBox(content, title, wxOK | wxICON_INFORMATION); // TODO: do we need icon?
}
