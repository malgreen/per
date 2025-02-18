#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class PerApp : public wxApp
{
public:
    virtual bool OnInit();
};