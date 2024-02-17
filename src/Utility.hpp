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
#include <boost/endian.hpp>

namespace MCPacker
{
    /// @brief Namespace with some utility function used by other parts of the program
    namespace Utility 
    {

        /// @brief Converts integral to a byte array with big-endian encoding
        /// @tparam Integral 
        /// @param n Number to convert
        /// @return Number as byte array
        template<typename Integral>
        std::array<uint8_t, sizeof(Integral)> ToByteArray(Integral n)
        {
            static_assert(std::is_integral<Integral>::value, "Argument must be of integral type");
            Integral inBigEndian = boost::endian::native_to_big(n);
            Integral* addr = std::addressof(inBigEndian);
            std::array<uint8_t, sizeof(Integral)> bytes;
            std::copy(reinterpret_cast<uint8_t*>(addr), reinterpret_cast<uint8_t*>(addr) + sizeof(inBigEndian), std::begin(bytes));
            return bytes;
        }

        /// @brief Convert big-endian byte arrat to an Integral type
        /// @tparam Integral 
        /// @param bytes Big-endian byte representation of a number
        /// @return Number represented by the byte array
        template<typename Integral>
        Integral FromByteArray(const std::array<uint8_t, sizeof(Integral)>& bytes)
        {
            static_assert(std::is_integral<Integral>::value, "Argument must be of integral type");
            if (bytes.size() != sizeof(Integral))
            {
                throw std::invalid_argument("Size of byte array must be equal to size of the desired type!");
            }
            Integral inBigEndian;
            std::copy(std::begin(bytes), std::end(bytes), reinterpret_cast<uint8_t*>(std::addressof(inBigEndian)));
            return boost::endian::big_to_native(inBigEndian);
        }
    }
}

#endif //UTILITY_HPP