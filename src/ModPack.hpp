/* 
 * This file is part of the MCPacker project (https://github.com/douaumont/MCPacker).
 * Copyright (c) 2024 Artyom Makarov.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
        static const std::u32string_view PackExt;

    private:
        std::u32string name, description;
        std::vector<Mod> mods;

    public:
        ModPack();
        ModPack(std::filesystem::path packFile);
        ModPack(std::u32string_view name, std::optional<std::u32string_view> description, const std::vector<std::filesystem::path>& modPaths);
        void AddMod(std::filesystem::path pathToJar);
        void WriteToFile(std::filesystem::path where) const;
    };
}

#endif //MODPACK_HPP