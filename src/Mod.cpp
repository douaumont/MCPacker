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

#include <fstream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <cuchar>
#include <array>
#include <boost/format.hpp>
#include "Mod.hpp"
#include "Utility.hpp"

using boost::format;
namespace fs = std::filesystem;

MCPacker::Mod::Mod(fs::path pathToJar)
{
    if (not fs::exists(pathToJar))
    {
        const auto message = format(u8"File %1% does not exist!") % std::quoted(pathToJar.u8string());
        throw std::invalid_argument(message.str());
    }

    {
        std::ifstream jar(pathToJar, std::ios::binary);
        if (jar.is_open())
        {
            data.reserve(fs::file_size(pathToJar));
            std::copy(std::istreambuf_iterator(jar), std::istreambuf_iterator<char>(),
                    std::back_inserter(data));
            name = pathToJar.filename().u32string();
        }
        else
        {
            const auto message = format(u8"Unable to read file %1%!") % pathToJar.filename().u8string();
            throw std::runtime_error(message.str());
        }
    }
}

void MCPacker::Mod::WriteToFile(std::ostream& modPackFile) const
{
    const auto dataSize = Utility::ToByteArray(data.size());
    const auto nameMultiByte = Utility::UTF32StringToMultiByte<sizeof(decltype(name)::value_type) * MaxModNameSize>(name);

    std::copy(std::begin(nameMultiByte), std::end(nameMultiByte), std::ostreambuf_iterator(modPackFile));
    std::copy(std::begin(dataSize), std::end(dataSize), std::ostreambuf_iterator(modPackFile));
    std::copy(std::begin(data), std::end(data), std::ostreambuf_iterator(modPackFile));
}