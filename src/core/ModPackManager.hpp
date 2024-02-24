#ifndef MOD_PACK_MANAGER_HPP
#define MOD_PACK_MANAGER_HPP

#include <filesystem>
#include <vector>
#include <boost/noncopyable.hpp>
#include "ModPack.hpp"

namespace MCPacker
{
    class ModPackManager final : private boost::noncopyable
    {
    private:
        std::filesystem::path pathToPacks;
        std::vector<ModPack> detectedModPacks;

        ModPackManager();

    public:
        static const ModPackManager& Instance();

        const std::vector<ModPack>& GetModPacks() const;
    };
}

#endif //MOD_PACK_MANAGER_HPP