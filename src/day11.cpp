/*
 *  <Short Description>
 *  Copyright (C) 2024  Brett Terpstra
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <days.h>
#include <blt/math/log_util.h>
#include <vector>
#include <limits>
#include <algorithm>
#include <blt/fs/loader.h>
#include <blt/iterator/enumerate.h>
#include <blt/iterator/iterator.h>
#include <blt/iterator/zip.h>
#include <blt/std/hashmap.h>
#include <blt/math/vectors.h>
#include <blt/std/assert.h>

void apply_rules(std::vector<blt::size_t>& stones)
{
    for (auto it = stones.begin(); it != stones.end(); ++it)
    {
        auto& val = *it;

        if (val == 0)
        {
            val = 1;
        } else if (auto str = std::to_string(val); str.size() % 2 == 0)
        {
            auto last = str.substr(str.size() / 2);
            auto first = str.substr(0, str.size() / 2);
            auto pos = stones.insert(stones.erase(it), std::stoull(last));
            auto pos2 = stones.insert(pos, std::stoull(first));
            it = ++pos2;
        } else
        {
            val *= 2024;
        }
    }
}

blt::hashmap_t<blt::vec2ul, blt::size_t> caches;

blt::size_t fuck_dynamic_programming(blt::size_t stone, blt::size_t depth)
{
    if (caches.contains(blt::vec2ul{stone, depth}))
        return caches[blt::vec2ul{stone, depth}];
    if (depth == 0)
        return 1;
    blt::size_t result = 0;
    if (stone == 0)
        result = fuck_dynamic_programming(1, depth - 1);
    else if (const auto str = std::to_string(stone); str.size() % 2 == 0)
    {
        result = fuck_dynamic_programming(std::stoull(str.substr(0, str.size() / 2)), depth - 1);
        result += fuck_dynamic_programming(std::stoull(str.substr(str.size() / 2)), depth - 1);
    } else
        result = fuck_dynamic_programming(stone * 2048, depth - 1);
    caches[blt::vec2ul{stone, depth}] = result;
    return result;
}

void print(const std::vector<blt::size_t>& stones)
{
    for (const auto i : stones)
        std::cout << i << " ";
    std::cout << std::endl;
}

void run_day11()
{
    auto file = blt::fs::getFile("../problems/day11/input");

    std::vector<blt::size_t> stones;

    auto stone_values = blt::string::split(file, ' ');
    for (const auto& v : stone_values)
        stones.push_back(std::stoull(v));

    auto part2_copy = stones;

    for (int i = 0; i < 25; i++)
        apply_rules(stones);

    BLT_TRACE(stones.size());

    blt::size_t total = 0;
    for (const auto i : part2_copy)
        total += fuck_dynamic_programming(i, 75);
    BLT_TRACE(total);

}