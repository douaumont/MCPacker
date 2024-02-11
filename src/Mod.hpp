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