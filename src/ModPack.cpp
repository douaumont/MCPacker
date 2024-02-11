#include <fstream>
#include <boost/format.hpp>
#include <boost/locale.hpp>
#include "ModPack.hpp"

using boost::format;

const std::u32string MCPacker::ModPack::PackExt = U".pck";

namespace 
{
    void EraseTrailingZeroes(std::u32string& str)
    {
        auto begin = std::rbegin(str);
        while (begin != std::rend(str) and *begin == U'\0')
        {
            ++begin;
        }
        str.erase(begin.base(), std::end(str));
    }
}

MCPacker::ModPack::ModPack()
{
    name.resize(NameLength);
    description.resize(DescriptionLength, U'\0');
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

    auto nameWithExt = name;
    EraseTrailingZeroes(nameWithExt);
    nameWithExt += PackExt;
    where /= nameWithExt;

    std::ofstream pack(where, std::ios::binary);
    std::copy(std::begin(name), std::end(name), std::ostreambuf_iterator(pack));
    std::copy(std::begin(description), std::end(description), std::ostreambuf_iterator(pack));
    std::for_each(std::begin(mods), std::end(mods), 
        [&pack](const Mod& mod)
        {
            mod.WriteToFile(pack);
        });
}