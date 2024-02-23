#include "ModPackManager.hpp"

MCPacker::ModPackManager::ModPackManager()
    :
    pathToPacks(std::filesystem::path(".") / std::filesystem::path("packs")),
    detectedModPacks()
{
    for (const auto& entry : std::filesystem::directory_iterator(pathToPacks))
    {
        if (entry.is_regular_file() and 
            entry.path().extension().u32string() == MCPacker::ModPack::MetaInfo::PackExt)
        {
            detectedModPacks.emplace_back(entry.path(), Utility::ReadingMode::OnlyMetaInfo);
        }
    }
}

const MCPacker::ModPackManager& MCPacker::ModPackManager::Instance()
{
    static ModPackManager instance;
    return instance;
}

const std::vector<MCPacker::ModPack>& MCPacker::ModPackManager::GetModPacks() const
{
    return detectedModPacks;
}
