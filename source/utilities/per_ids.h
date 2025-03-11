#ifndef PER_IDS_H_
#define PER_IDS_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/aui/auibook.h>

enum
{
    perID_ZERO, // id of zero doesn't work for some widgets on macOS
    perID_NEW_TAB
};

#endif
