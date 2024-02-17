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
#include <array>
#include <boost/format.hpp>
#include <boost/locale.hpp>
#include "Mod.hpp"
#include "Utility.hpp"

using boost::format;
using namespace boost::locale::conv;
namespace fs = std::filesystem;

MCPacker::Mod::Mod(fs::path pathToJar)
{
    if (not fs::exists(pathToJar))
    {
        const auto message = format("File %1% does not exist!") % std::quoted(pathToJar.string());
        throw std::invalid_argument(message.str());
    }

    {
        std::ifstream jar(pathToJar, std::ios::binary);
        if (jar.is_open())
        {
            data.reserve(fs::file_size(pathToJar));
            std::copy(std::istreambuf_iterator(jar), std::istreambuf_iterator<char>(),
                    std::back_inserter(data));
            auto&& p = pathToJar.filename().u32string();
            std::copy(std::begin(p), std::end(p), std::begin(metaInfo.name));
        }
        else
        {
            const auto message = format("Unable to read file %1%!") % pathToJar.filename().string();
            throw std::runtime_error(message.str());
        }
    }
}

void MCPacker::Mod::WriteToFile(std::ostream& modPackFile) const
{
    const auto dataSize = Utility::ToByteArray(data.size());
    std::u32string name(std::begin(metaInfo.name), std::end(metaInfo.name));
    auto narrowName = utf_to_utf<char>(name);
    narrowName.resize(MetaInfo::NameLengthInBytes, '\0');

    std::copy(std::begin(narrowName), std::end(narrowName), std::ostreambuf_iterator(modPackFile));
    std::copy(std::begin(dataSize), std::end(dataSize), std::ostreambuf_iterator(modPackFile));
    std::copy(std::begin(data), std::end(data), std::ostreambuf_iterator(modPackFile));
}