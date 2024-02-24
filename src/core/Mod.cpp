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
#include <ranges>
#include <type_traits>
#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "Mod.hpp"
#include "Utility.hpp"

using boost::format;
using namespace MCPacker::Utility::Definitions;
namespace fs = std::filesystem;

MCPacker::Mod::MetaInfo::MetaInfo() 
    :
    name()
{
    name.fill(0);
}

MCPacker::Mod::Mod(fs::path pathToJar)
    :
    metaInfo(),
    data()
{
    if (not fs::exists(pathToJar))
    {
        const auto message = format("File %1% does not exist!") % std::quoted(pathToJar.string());
        throw std::invalid_argument(message.str());
    }

    {
        InputBinaryFile jar(pathToJar, std::ios::binary);
        if (jar.is_open())
        {
            data.reserve(fs::file_size(pathToJar));
            std::copy(std::istreambuf_iterator(jar), std::istreambuf_iterator<Byte>(),
                    std::back_inserter(data));
            std::ranges::copy(pathToJar.filename().u32string(), std::begin(metaInfo.name));
        }
        else
        {
            const auto message = format("Unable to read file %1%!") % pathToJar.filename().string();
            throw std::runtime_error(message.str());
        }
    }
}

MCPacker::Mod::Mod(InputBinaryFile& pack, Utility::ReadingMode readingMode)
    :
    metaInfo(),
    data()
{
    std::array<Byte, MetaInfo::NameLengthInBytes> name;
    std::array<Byte, sizeof(uint64_t)> dataSizeSerialised;

    pack.read(name.data(), name.size());
    pack.read(dataSizeSerialised.data(), dataSizeSerialised.size());
    metaInfo.name = Utility::FromUTF8Array(name);
    const auto dataSize = Utility::FromByteArray<uint64_t>(dataSizeSerialised);

    switch (readingMode)
    {
        case Utility::ReadingMode::Full:
            data.resize(dataSize);
            pack.read(data.data(), data.size());
            break;
        
        case Utility::ReadingMode::OnlyMetaInfo:
            pack.seekg(pack.tellg() + boost::numeric_cast<std::remove_cvref_t<decltype(pack)>::pos_type>(dataSize));
            break;

        default:
            throw std::invalid_argument("Unrecognised reading mode");
    }
}

void MCPacker::Mod::WriteToPack(OutputBinaryFile& modPackFile) const
{
    const auto dataSize = Utility::ToByteArray(static_cast<uint64_t>(data.size()));
    const auto name = Utility::ToUTF8Array<MetaInfo::NameLength>(std::u32string_view(std::begin(metaInfo.name), std::end(metaInfo.name)));

    std::copy(std::begin(name), std::end(name), std::ostreambuf_iterator(modPackFile));
    std::copy(std::begin(dataSize), std::end(dataSize), std::ostreambuf_iterator(modPackFile));
    std::copy(std::begin(data), std::end(data), std::ostreambuf_iterator(modPackFile));
}

void MCPacker::Mod::WriteToFile(std::filesystem::path where) const
{
    auto end = std::ranges::find_if(metaInfo.name, Utility::EqualsZero<char32_t>());
    auto pathToJar = where / std::u32string_view(std::begin(metaInfo.name), end);

    OutputBinaryFile jar(pathToJar, std::ios::binary | std::ios::out | std::ios::trunc);
    jar.write(data.data(), data.size());
}