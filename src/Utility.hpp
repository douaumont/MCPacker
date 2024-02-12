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

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <array>
#include <type_traits>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <cuchar>
#include <boost/endian.hpp>

namespace MCPacker
{
    namespace Utility 
    {
        template<typename Integral>
        std::array<uint8_t, sizeof(Integral)> ToByteArray(Integral n)
        {
            static_assert(std::is_integral<Integral>::value, u8"Argument must be of integral type");
            Integral inBigEndian = boost::endian::native_to_big(n);
            Integral* addr = std::addressof(inBigEndian);
            std::array<uint8_t, sizeof(Integral)> bytes;
            std::copy(reinterpret_cast<uint8_t*>(addr), reinterpret_cast<uint8_t*>(addr) + sizeof(inBigEndian), std::begin(bytes));
            return bytes;
        }

        template<typename Integral>
        Integral FromByteArray(const std::array<uint8_t, sizeof(Integral)>& bytes)
        {
            static_assert(std::is_integral<Integral>::value, u8"Argument must be of integral type");
            if (bytes.size() != sizeof(Integral))
            {
                throw std::invalid_argument(u8"Size of byte array must be equal to size of the desired type!");
            }
            Integral inBigEndian;
            std::copy(std::begin(bytes), std::end(bytes), reinterpret_cast<uint8_t*>(std::addressof(inBigEndian)));
            return boost::endian::big_to_native(inBigEndian);
        }

        template<size_t N>
        std::array<char, N> UTF32StringToMultiByte(std::u32string_view str)
        {
            std::array<char, N> strMultiByte;
            std::for_each(std::begin(str), std::end(str), 
                [&strMultiByte](char32_t c)
                {
                    std::mbstate_t state{};
                    std::c32rtomb(strMultiByte.data(), c, std::addressof(state));
                });
            return strMultiByte;
        }
    }
}

#endif //UTILITY_HPP