#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    wxDECLARE_EVENT_TABLE();
    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void BuildMenuBar();
};

#endif