#include <fstream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <boost/format.hpp>
#include "Mod.hpp"

using boost::format;

MCPacker::Mod::Mod(std::filesystem::path pathToJar)
{
    if (not std::filesystem::exists(pathToJar))
    {
        const auto message = format(u8"File %1% does not exist!") % std::quoted(pathToJar.u8string());
        throw std::invalid_argument(message.str());
    }

    {
        std::ifstream jar(pathToJar, std::ios::binary);
        if (jar.is_open())
        {
            data.reserve(std::filesystem::file_size(pathToJar));
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
    std::copy(std::begin(name), std::end(name), std::ostreambuf_iterator(modPackFile));

    uint64_t dataSize = data.size();
    modPackFile.write(reinterpret_cast<char*>(std::addressof(dataSize)), sizeof(dataSize));

    std::copy(std::begin(data), std::end(data), std::ostreambuf_iterator(modPackFile));
}