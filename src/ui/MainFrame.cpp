#include <algorithm>
#include <ranges>
#include <core/ModPackManager.hpp>
#include "MainFrame.hpp"

MainFrame::MainFrame()
    :
    wxFrame(nullptr, wxID_ANY, L"MCPacker")
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    modPackList = new wxListBox(this, wxID_ANY);
    const auto& modPackManager = MCPacker::ModPackManager::Instance();
    wxArrayString modPacks;
    std::ranges::for_each(modPackManager.GetModPacks(), 
        [&modPacks](const auto& modPack)
        {
            modPacks.push_back(wxString::FromUTF8(modPack.GetMetaInfo().GetNameInUTF8()));
        });
    modPackList->Append(modPacks);
    sizer->Add(modPackList);
    SetSizer(sizer);
}