#ifndef PER_IDS_H_
#define PER_IDS_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/aui/auibook.h>

// id of zero doesn't work for some widgets on macOS
enum
{
    perID_NONE = -1,
    perID_NEW_TAB = 1,
    perID_HTTP_PARAMS_GRID = 2,

};

#endif
