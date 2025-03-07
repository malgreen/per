#include "wx/button.h"
#include "wx/sizer.h"
#include <wx/aui/auibook.h>
#include <cstddef>
#include "main_frame.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
        EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
            wxEND_EVENT_TABLE();

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    this->BuildMenuBar();

    wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);

    wxComboBox *comboBox = new wxComboBox(this, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
    wxString httpMethods[] = {"GET", "POST", "PUT", "PATCH", "DELETE", "HEAD", "OPTIONS"};
    for (wxString method : httpMethods)
    {
        comboBox->Append(method);
    }
    comboBox->SetSelection(0);

    boxSizer->Add(comboBox, 0, wxALL, 5);

    wxTextCtrl *textCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    boxSizer->Add(textCtrl, 0, wxEXPAND, 5);

    wxButton *button = new wxButton(this, wxID_ANY, _("Send"), wxDefaultPosition, wxDefaultSize, 0);
    boxSizer->Add(button, 0, wxALL, 5);

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

void MainFrame::BuildMenuBar()
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
}
