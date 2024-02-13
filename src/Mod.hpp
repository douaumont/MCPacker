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
    private:
        
        /// @brief Mod's name in UTF-32 encoding
        std::u32string name;
        
        /// @brief Binary content of the mod 
        std::vector<uint8_t> data;

    public:
        /// @brief Construct a mod from the corresponding .jar file
        /// @param pathToJar
        Mod(std::filesystem::path pathToJar);

        /// @brief Write this mod's representation into ModPack file
        /// @param modPackFile 
        /// @warning This function is only used by ModPack class
        /// Normally it shouldn't be called from anywhere else
        void WriteToFile(std::ostream& modPackFile) const;
    };
}

#endif //MOD_HPP