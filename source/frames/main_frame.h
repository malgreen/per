#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

#include <map>
#include <wx/wx.h>
#include <wx/aui/auibook.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

protected:
    wxAuiNotebook *m_TabNotebook;
    std::map<int, int> m_TabActionMap;

private:
    wxDECLARE_EVENT_TABLE();
    void BuildMenuBar();
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnNewTab(wxCommandEvent &event);
    void OnCloseTab(wxCommandEvent &event);
    void OnSwitchTab(wxCommandEvent &event);
    void CreateTab();
};

#endif
