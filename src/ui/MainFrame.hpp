#ifndef MAIN_FRAME_HPP
#define MAIN_FRAME_HPP

#include <wx/wx.h>

class MainFrame : public wxFrame
{
private:
    wxListBox* modPackList;

public:
    MainFrame();
};

#endif //MAIN_FRAME_HPP