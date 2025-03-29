#include "per_aui_tab_art.h"

// TODO:
// - improve SVGs
// - load from disk, maybe at build time

PerAuiTabArt::PerAuiTabArt()
{
    SetAppropriateCloseButton();
}

PerAuiTabArt::~PerAuiTabArt()
{
}

wxAuiTabArt *PerAuiTabArt::Clone()
{
    return new PerAuiTabArt(*this);
}

void PerAuiTabArt::UpdateColoursFromSystem()
{
    wxAuiDefaultTabArt::UpdateColoursFromSystem(); // call base method
    SetAppropriateCloseButton();
}

void PerAuiTabArt::SetAppropriateCloseButton()
{
    char closeSvgBlack[] = R"(<svg xmlns="http://www.w3.org/2000/svg"><path fill="black" d="M289.94,256l95-95A24,24,0,0,0,351,127l-95,95-95-95A24,24,0,0,0,127,161l95,95-95,95A24,24,0,1,0,161,385l95-95,95,95A24,24,0,0,0,385,351Z"/></svg>)";
    char closeSvgWhite[] = R"(<svg xmlns="http://www.w3.org/2000/svg"><path fill="white" d="M289.94,256l95-95A24,24,0,0,0,351,127l-95,95-95-95A24,24,0,0,0,127,161l95,95-95,95A24,24,0,1,0,161,385l95-95,95,95A24,24,0,0,0,385,351Z"/></svg>)";

    auto closeBmp = wxBitmapBundle::FromSVG(wxSystemSettings::GetAppearance().IsDark() ? closeSvgWhite : closeSvgBlack, wxSize(12, 12));

    m_activeCloseBmp = closeBmp;
    m_disabledCloseBmp = closeBmp;
}
