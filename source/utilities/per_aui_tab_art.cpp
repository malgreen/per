#include "per_aui_tab_art.h"

// TODO:
// - make these private static members of PerAuiTabArt
// - find out if we can load SVG from disk on build time (maybe with CMake?)
static char m_closeButtonSvg[] = R"(<svg xmlns="http://www.w3.org/2000/svg"><path d="M289.94,256l95-95A24,24,0,0,0,351,127l-95,95-95-95A24,24,0,0,0,127,161l95,95-95,95A24,24,0,1,0,161,385l95-95,95,95A24,24,0,0,0,385,351Z"/></svg>)";
static wxBitmapBundle m_closeButtonBmp = wxBitmapBundle::FromSVG(m_closeButtonSvg, wxSize(12, 12));

PerAuiTabArt::PerAuiTabArt()
{
    m_activeCloseBmp = m_closeButtonBmp;
    m_disabledCloseBmp = m_closeButtonBmp;
}

wxAuiTabArt *PerAuiTabArt::Clone()
{
    return new PerAuiTabArt(*this);
}
