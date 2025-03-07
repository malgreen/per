#ifndef HTTP_REQUEST_MODEL_H_
#define HTTP_REQUEST_MODEL_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

typedef struct
{
    wxString url;

} HttpRequestModel;

// class HttpRequestModel
// {
// public:
//     MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

// private:
//     wxDECLARE_EVENT_TABLE();
//     void OnExit(wxCommandEvent &event);
//     void OnAbout(wxCommandEvent &event);
//     void BuildMenuBar();
// };

#endif