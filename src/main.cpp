#include <wx/wx.h>
#include "ui/MainFrame.hpp"

class MCPackerApp : public wxApp
{
public:
    bool OnInit() override 
    {
        if (not wxApp::OnInit())
        {
            return false;
        }
        auto* mainFrame = new MainFrame();
        mainFrame->Show();
        SetTopWindow(mainFrame);
        return true;
    }
};

wxIMPLEMENT_APP(MCPackerApp);