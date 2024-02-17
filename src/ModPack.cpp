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
#include <boost/format.hpp>
#include "ModPack.hpp"
#include "Utility.hpp"

using boost::format;
using namespace MCPacker::Utility::Definitions;

const std::u32string_view MCPacker::ModPack::MetaInfo::PackExt = U".pck";

MCPacker::ModPack::MetaInfo::MetaInfo()
{
    name.fill(0);
    description.fill(0);
}

MCPacker::ModPack::ModPack()
{

}

MCPacker::ModPack::ModPack(std::filesystem::path packFile)
{
    InputBinaryFile pack(packFile);

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

    while(not pack.eof())
    {
        mods.emplace_back(pack);
    }
}

MCPacker::ModPack::ModPack(std::u32string_view name, std::optional<std::u32string_view> description, const std::vector<std::filesystem::path>& modPaths)
    :
    ModPack()
{
    std::copy(std::begin(name), std::end(name), std::begin(metaInfo.name));
    
    if (description.has_value())
    {
        std::copy(std::begin(*description), std::end(*description), std::begin(metaInfo.description));
    }

    std::for_each(std::begin(modPaths), std::end(modPaths), 
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
    std::ranges::copy_if(metaInfo.name, std::back_inserter(nameWithExt), Utility::NotEqualsZero<char32_t>());
    std::ranges::copy(MetaInfo::PackExt, std::back_inserter(nameWithExt));

    where /= nameWithExt;

    OutputBinaryFile pack(where, std::ios::binary);

    std::ranges::copy(Utility::ToUTF8Array<MetaInfo::NameLength>(std::u32string_view(std::begin(metaInfo.name), std::end(metaInfo.name))), std::ostreambuf_iterator(pack));
    std::ranges::copy(Utility::ToUTF8Array<MetaInfo::DescriptionLength>(std::u32string_view(std::begin(metaInfo.description), std::end(metaInfo.description))), std::ostreambuf_iterator(pack));
    std::for_each(std::begin(mods), std::end(mods), 
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