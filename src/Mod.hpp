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

    class Mod 
    {
    private:
        std::u32string name;
        std::vector<uint8_t> data;

    public:
        Mod(std::filesystem::path pathToJar);
        void WriteToFile(std::ostream& modPackFile) const;
    };
}

#endif //MOD_HPP