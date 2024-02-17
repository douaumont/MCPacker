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

#ifndef MOD_HPP
#define MOD_HPP

#include <array>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <string>
#include <array>

namespace MCPacker
{
    constexpr size_t MaxModNameSize = 255;

    /// @brief Class that represents a mod
    /// @details This class stores a mod as a binary array and the mod's name
    class Mod 
    {
    public:
        /// @brief Struct that stores meta information about a mod
        struct MetaInfo
        {
            static constexpr size_t NameLength = 255;
            static constexpr size_t NameLengthInBytes = NameLength * sizeof(char32_t);

            /// @brief Mod's name in UTF-32 encoding
            std::array<char32_t, NameLength> name;    
        };

    private:
        MetaInfo metaInfo;

        /// @brief Binary content of the mod 
        std::vector<Utility::Definitions::Byte> data;

    public:
        /// @brief Construct a mod from the corresponding `.jar` file
        /// @param pathToJar
        Mod(std::filesystem::path pathToJar);


        /// @brief Construct from a `pack` file
        /// @param pack 
        Mod(Utility::Definitions::InputBinaryFile& pack);

        /// @brief Write this mod's representation into `ModPack` file
        /// @param modPackFile 
        /// @warning This function is only used by ModPack class
        /// Normally it shouldn't be called from anywhere else
        void WriteToPack(Utility::Definitions::OutputBinaryFile& modPackFile) const;

        void WriteToFile(std::filesystem::path where) const;
    };
}

#endif //MOD_HPP