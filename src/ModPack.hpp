#ifndef MODPACK_HPP
#define MODPACK_HPP

#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include "Mod.hpp"

namespace MCPacker
{
    class ModPack
    {
    public:
        static constexpr size_t NameLength = 255, DescriptionLength = 2048;
        static const std::u32string PackExt;

    private:
        std::u32string name, description;
        std::vector<Mod> mods;

    public:
        ModPack();
        ModPack(std::u32string_view name, std::optional<std::u32string_view> description, const std::vector<std::filesystem::path>& modPaths);
        void AddMod(std::filesystem::path pathToJar);
        void WriteToFile(std::filesystem::path where) const;
    };
}

#endif //MODPACK_HPP