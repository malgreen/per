#ifndef PER_APP_H
#define PER_APP_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class PerApp : public wxApp
{
public:
    virtual bool OnInit();
};
#endif