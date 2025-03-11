#ifndef HTTP_PANEL_H_
#define HTTP_PANEL_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../models/http_models.h"

class HttpPanel : public wxPanel
{
public:
    HttpPanel(wxWindow *parent, HttpRequestModel *httpRequestModel);

protected:
    wxComboBox *httpMethodComboBox;

private:
    wxDECLARE_EVENT_TABLE();
    wxComboBox *BuildHttpMethodComboBox();
};

#endif