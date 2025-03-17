#ifndef HTTP_PANEL_H_
#define HTTP_PANEL_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "../models/http_models.h"
#include <wx/notebook.h>
#include <wx/grid.h>
#include "../utilities/per_ids.h"

class HttpPanel : public wxPanel
{
public:
    HttpPanel(wxWindow *parent, HttpRequestModel *httpRequestModel);

protected:
    wxComboBox *m_HttpMethodComboBox;
    wxGrid *m_ParamsGrid;
    wxGrid *m_HeadersGrid;

private:
    wxDECLARE_EVENT_TABLE();
    wxComboBox *BuildHttpMethodComboBox();
    wxPanel *BuildParamsPanel(wxWindow *parent);
    wxPanel *BuildHeadersPanel(wxWindow *parent);
    wxPanel *BuildBodyPanel(wxWindow *parent);
    void OnGridCellChange(wxGridEvent &event);
};

#endif
