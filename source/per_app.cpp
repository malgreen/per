#include "per_app.h"
#include "frames/main_frame.h"
#include "utilities/about.h"

wxIMPLEMENT_APP(PerApp);

bool PerApp::OnInit()
{
    MainFrame *frame = new MainFrame(PROJECT_NAME, wxPoint(50, 50), wxSize(1280, 720));
    frame->Show(true);
    return true;
}
