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
#include <boost/locale.hpp>
#include "ModPack.hpp"
#include "Utility.hpp"

using boost::format;
using namespace boost::locale::conv;

const std::u32string_view MCPacker::ModPack::PackExt = U".pck";

MCPacker::ModPack::ModPack()
{
    name.resize(NameLength);
    description.resize(DescriptionLength, U'\0');
}

MCPacker::ModPack::ModPack(std::filesystem::path packFile)
{

}

MCPacker::ModPack::ModPack(std::u32string_view name, std::optional<std::u32string_view> description, const std::vector<std::filesystem::path>& modPaths)
    :
    ModPack()
{
    std::copy(std::begin(name), std::end(name), std::begin(this->name));
    
    if (description.has_value())
    {
        std::copy(std::begin(*description), std::end(*description), std::begin(this->description));
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
        const auto message = format(u8"Path %1% is not a directory!") % std::quoted(where.u8string());
        throw std::invalid_argument(message.str());
    }

    std::u32string nameWithExt;
    std::copy_if(std::begin(name), std::end(name), std::back_inserter(nameWithExt), 
        [](char32_t c) {return c != U'\0';});
    std::copy(std::begin(PackExt), std::end(PackExt), std::back_inserter(nameWithExt));

    where /= nameWithExt;

    std::ofstream pack(where, std::ios::binary);
    constexpr auto sizeOfNameInBytes = sizeof(decltype(name)::value_type) * NameLength;
    constexpr auto sizeOfDescriptionInBytes = sizeof(decltype(description)::value_type) * DescriptionLength;

    auto narrowName = utf_to_utf<char>(name);
    narrowName.resize(sizeOfNameInBytes, 0);
    auto narrowDescription = utf_to_utf<char>(description);
    narrowDescription.resize(sizeOfDescriptionInBytes);

    std::copy(std::begin(narrowName), std::end(narrowName), std::ostreambuf_iterator(pack));
    std::copy(std::begin(narrowDescription), std::end(narrowDescription), std::ostreambuf_iterator(pack));
    std::for_each(std::begin(mods), std::end(mods), 
        [&pack](const Mod& mod)
        {
            mod.WriteToFile(pack);
        });
}