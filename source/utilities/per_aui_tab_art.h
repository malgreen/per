#ifndef PER_AUI_TAB_ART_H_
#define PER_AUI_TAB_ART_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/aui/auibook.h>

class PerAuiTabArt : public wxAuiDefaultTabArt
{
public:
    PerAuiTabArt();
    virtual ~PerAuiTabArt(); // TODO: do we need destructrs?
    wxAuiTabArt *Clone() override;
    void UpdateColoursFromSystem() override;

private:
    void SetAppropriateCloseButton();
};

#endif
