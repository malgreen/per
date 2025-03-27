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
    wxAuiTabArt *Clone();

    // private:
    //     static const char *m_closeButtonSvg;
};

#endif
