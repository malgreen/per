#include "per_app.h"
#include "frames/main_frame.h"

bool PerApp::OnInit()
{
    MainFrame *frame = new MainFrame("Per", wxPoint(50, 50), wxSize(1280, 720));
    frame->Show(true);
    return true;
}
