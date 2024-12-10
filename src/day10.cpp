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



namespace day10
{

    std::vector<std::vector<int>> topo;
    blt::hashset_t<blt::vec2i> positions;
    blt::i32 width, height;
    int rating = 0;

    void search(std::vector<std::vector<int>>& vec, const blt::i32 x, const blt::i32 y)
    {
        const auto val = topo[y][x];
        // BLT_TRACE(val);
        if (val == 9)
        {
            rating++;
            positions.insert(blt::vec2i{x, y});
            return;
        }
        if (x + 1 < width && topo[y][x + 1] - val == 1)
            search(vec, x + 1, y);
        if (x - 1 >= 0 && topo[y][x - 1] - val == 1)
            search(vec, x - 1, y);
        if (y + 1 < height && topo[y + 1][x] - val == 1)
            search(vec, x, y+1);
        if (y - 1 >= 0 && topo[y - 1][x] - val == 1)
            search(vec, x, y-1);
    }

    int run_search(const blt::i32 x, const blt::i32 y)
    {
        auto copy = topo;
        search(copy, x, y);
        const auto total = positions.size();
        positions.clear();
        return static_cast<int>(total);
    }

}

void run_day10()
{
    using namespace day10;

    auto file = blt::fs::getLinesFromFile("../problems/day10/input");

    for (const auto& line : file)
    {
        topo.emplace_back();
        for (const auto c : line)
            topo.back().push_back(c - '0');
    }

    height = static_cast<blt::i32>(topo.size());
    width = static_cast<blt::i32>(topo.begin()->size());

    int total = 0;
    for (blt::i32 j = 0; j < height; j++)
    {
        for (blt::i32 i = 0; i < width; i++)
        {
            if (topo[j][i] == 0)
            {
                total += run_search(i, j);
            }
        }
    }

    BLT_TRACE(total);
    BLT_TRACE(rating);

}