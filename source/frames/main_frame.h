#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/aui/auibook.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

protected:
    wxAuiNotebook *m_TabNotebook;

private:
    wxDECLARE_EVENT_TABLE();
    void BuildMenuBar();
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnNewTab(wxCommandEvent &event);
};

enum
{
    perID_ZERO, // id of zero doesn't work for some widgets on macOS
    perID_NEW_TAB
};

#endif
