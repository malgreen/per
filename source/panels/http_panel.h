#ifndef HTTP_PANEL_H_
#define HTTP_PANEL_H_

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/notebook.h>
#include "../utilities/per_ids.h"
#include "../models/http_models.h"

class HttpPanel : public wxPanel
{
public:
    HttpPanel(wxWindow *parent, HttpRequestModel *httpRequestModel);

protected:
    wxComboBox *m_HttpMethodComboBox;
    wxGrid *m_ParamsGrid;
    wxGrid *m_HeadersGrid;

private:
    HttpRequestModel *m_HttpRequestModel;
    wxDECLARE_EVENT_TABLE();
    wxComboBox *BuildHttpMethodComboBox();
    wxPanel *BuildParamsPanel(wxWindow *parent);
    wxPanel *BuildHeadersPanel(wxWindow *parent);
    wxPanel *BuildBodyPanel(wxWindow *parent);
    void OnGridCellChange(wxGridEvent &event);
    void OnSize(wxSizeEvent &event);
    void AutoFitGrids();
};

#endif
