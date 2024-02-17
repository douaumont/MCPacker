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
#include <concepts>
#include <ranges>
#include <utf8.h>
#include <boost/endian.hpp>

namespace MCPacker
{
    /// @brief Namespace with some utility function used by other parts of the program
    namespace Utility 
    {
        namespace Definitions
        {
            using Byte = uint8_t;
            using InputBinaryFile = std::basic_ifstream<Byte>;
            using OutputBinaryFile = std::basic_ofstream<Byte>;
        }
        
        /// @brief Servers as a predicate
        /// @tparam T any equiality comparable type
        template <typename T>
        requires(std::equality_comparable<T>)
        struct EqualsZero 
        {
            bool operator()(const T& value) const
            {
                return value == static_cast<T>(0);
            }
        };

        /// @brief Servers as a predicate
        /// @tparam T any equiality comparable type
        template <typename T>
        requires(std::equality_comparable<T>)
        struct NotEqualsZero 
        {
            bool operator()(const T& value) const
            {
                return value != static_cast<T>(0);
            }
        };


        /// @brief Converts integral to a byte array with big-endian encoding
        /// @tparam T 
        /// @param n Number to convert
        /// @return Number as byte array
        template<typename T>
        requires(std::is_integral_v<T>)
        std::array<uint8_t, sizeof(T)> ToByteArray(T n)
        {
            T inBigEndian = boost::endian::native_to_big(n);
            T* addr = std::addressof(inBigEndian);
            std::array<uint8_t, sizeof(T)> bytes;
            std::copy(reinterpret_cast<uint8_t*>(addr), reinterpret_cast<uint8_t*>(addr) + sizeof(inBigEndian), std::begin(bytes));
            return bytes;
        }

        /// @brief Convert big-endian byte arrat to an T type
        /// @tparam T 
        /// @param bytes Big-endian byte representation of a number
        /// @return Number represented by the byte array
        template<typename T>
        requires(std::is_integral_v<T>)
        T FromByteArray(const std::array<uint8_t, sizeof(T)>& bytes)
        {
            if (bytes.size() != sizeof(T))
            {
                throw std::invalid_argument("Size of byte array must be equal to size of the desired type!");
            }
            T inBigEndian;
            std::copy(std::begin(bytes), std::end(bytes), reinterpret_cast<uint8_t*>(std::addressof(inBigEndian)));
            return boost::endian::big_to_native(inBigEndian);
        }

        
        /// @brief Converts UTF-32 encoded string into array of bytes in UTF-8 encoding
        /// @tparam N number of characters in `str`
        /// @param str string to converto into byte array
        /// @return UTF-8 encoded byte array
        template<size_t N>
        std::array<uint8_t, N * sizeof(char32_t)> ToUTF8Array(std::u32string_view str)
        {
            std::array<uint8_t, N * sizeof(char32_t)> utf8str;
            utf8str.fill(0);
            utf8::utf32to8(std::begin(str), std::end(str), std::begin(utf8str));
            return utf8str;
        }

        /// @brief Converts UTF-8 encoded byte array to UTF-32 encoded array
        /// @tparam N number of bytes in `utf8str`
        /// @param utf8str UTF-8 encoded byte array
        /// @return array of UTF-32 characters
        /// @warning `N` must be divisible by 4, i.e. `sizeof(char32_t)`
        template<size_t N>
        requires(N % sizeof(char32_t) == 0)
        std::array<char32_t, N / sizeof(char32_t)> FromUTF8Array(const std::array<uint8_t, N>& utf8str)
        {
            std::array<char32_t, N / sizeof(char32_t)> utf32str;
            utf32str.fill(0);
            auto end = std::ranges::find_if(utf8str, EqualsZero<uint8_t>());
            utf8::utf8to32(std::begin(utf8str), end, std::begin(utf32str));
            return utf32str;
        } 
    }
}

#endif //UTILITY_HPP