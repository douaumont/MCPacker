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
#include <algorithm>
#include <ranges>
#include <boost/format.hpp>
#include <boost/algorithm/cxx11/copy_if.hpp>
#include "ModPack.hpp"
#include "Utility.hpp"

using boost::format;
using namespace MCPacker::Utility::Definitions;
using MCPacker::Utility::ReadingMode;

const std::u32string_view MCPacker::ModPack::MetaInfo::PackExt = U".pck";

MCPacker::ModPack::MetaInfo::MetaInfo()
    :
    name(),
    description()
{
    name.fill(0);
    description.fill(0);
}

std::string MCPacker::ModPack::MetaInfo::GetNameInUTF8() const
{
    return Utility::UTF32ArrayToUTF8String(name);
}

MCPacker::ModPack::ModPack()
    :
    metaInfo(),
    mods()
{

}

MCPacker::ModPack::ModPack(std::filesystem::path packFile, Utility::ReadingMode readingMode)
{
    InputBinaryFile pack(packFile);
    const auto packSize = std::filesystem::file_size(packFile);

    {
        std::array<Byte, MetaInfo::NameLength * sizeof(char32_t)> name;
        std::array<Byte, MetaInfo::DescriptionLength * sizeof(char32_t)> description;
        name.fill(0);
        description.fill(0);

        pack.read(name.data(), name.size());
        pack.read(description.data(), description.size());

        metaInfo.name = Utility::FromUTF8Array(name);
        metaInfo.description = Utility::FromUTF8Array(description);
    }

    while(pack.tellg() < packSize)
    {
        mods.emplace_back(pack, readingMode);
    }
}

MCPacker::ModPack::ModPack(std::u32string_view name, std::optional<std::u32string_view> description, const std::vector<std::filesystem::path>& modPaths)
    :
    ModPack()
{
    std::ranges::copy(name, std::begin(metaInfo.name));
    
    if (description.has_value())
    {
        std::ranges::copy(*description, std::begin(metaInfo.description));
    }

    std::ranges::for_each(modPaths, 
        [this](const auto& path)
        {
            AddMod(path);
        });
}

void MCPacker::ModPack::AddMod(std::filesystem::path pathToJar)
{
    mods.emplace_back(pathToJar);
}

void MCPacker::ModPack::WriteToFile(std::filesystem::path where) const
{
    if (not std::filesystem::is_directory(where))
    {
        const auto message = format("Path %1% is not a directory!") % std::quoted(where.string());
        throw std::invalid_argument(message.str());
    }

    std::u32string nameWithExt;
    boost::algorithm::copy_until(metaInfo.name, std::back_inserter(nameWithExt), Utility::NotEqualsZero<char32_t>());
    std::ranges::copy(MetaInfo::PackExt, std::back_inserter(nameWithExt));

    where /= nameWithExt;

    OutputBinaryFile pack(where, std::ios::binary);

    std::ranges::copy(Utility::ToUTF8Array<MetaInfo::NameLength>(std::u32string_view(std::begin(metaInfo.name), std::end(metaInfo.name))), std::ostreambuf_iterator(pack));
    std::ranges::copy(Utility::ToUTF8Array<MetaInfo::DescriptionLength>(std::u32string_view(std::begin(metaInfo.description), std::end(metaInfo.description))), std::ostreambuf_iterator(pack));
    std::ranges::for_each(mods, 
        [&pack](const Mod& mod)
        {
            mod.WriteToPack(pack);
        });
}

void MCPacker::ModPack::Deploy(std::filesystem::path where) const
{
    if (not std::filesystem::exists(where))
    {
        std::filesystem::create_directory(where);
    }

    std::ranges::for_each(mods, 
        [&where](const MCPacker::Mod& mod)
        {
            mod.WriteToFile(where);
        });
}

const MCPacker::ModPack::MetaInfo& MCPacker::ModPack::GetMetaInfo() const
{
    return metaInfo;
}