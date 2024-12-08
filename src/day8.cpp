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
#include <day8.h>
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

namespace day8
{
    std::vector<char> map;
    std::vector<char> antinodes;
    std::vector<char> antinodes2;

    int width, height;

    inline char& get(const int i, const int j)
    {
        return map[j * width + i];
    }

    inline void anti_set(const char c, const int i, const int j)
    {
        if (j >= height || i >= width || i < 0 || j < 0)
            return;
        antinodes[j * width + i] = c;
    }

    inline bool anti_set2(const char c, const int i, const int j)
    {
        if (j >= height || i >= width || i < 0 || j < 0)
            return false;
        antinodes2[j * width + i] = c;
        return true;
    }

    void run(const char compare, const int x, const int y)
    {
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                if (i == x && y == j)
                    continue;
                const auto current = get(i, j);

                if (compare == current)
                {
                    const int dist_x = i - x;
                    const int dist_y = j - y;

                    anti_set('#', i + dist_x, j + dist_y);
                    anti_set('#', x - dist_x, y - dist_y);
                }
            }
        }
    }

    void run2(const char compare, const int x, const int y)
    {
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                if (i == x && y == j)
                    continue;
                const auto current = get(i, j);

                if (compare == current)
                {
                    const int dist_x = i - x;
                    const int dist_y = j - y;

                    for (int mul = 0; mul < width * height; mul++)
                    {
                        if (!anti_set2('#', i + (dist_x * mul), j + (dist_y * mul)) && !anti_set2('#', x - (dist_x * mul), y - (dist_y * mul)))
                            break;
                    }
                }
            }
        }
    }
}

void run_day8()
{
    using namespace day8;

    auto lines = blt::fs::getLinesFromFile("../problems/day8/input");

    height = static_cast<int>(lines.size());
    width = static_cast<int>(lines.begin()->size());
    for (const auto& line : lines)
    {
        map.insert(map.begin(), line.begin(), line.end());
        antinodes.insert(antinodes.begin(), line.begin(), line.end());
        antinodes2.insert(antinodes2.begin(), line.begin(), line.end());
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            const auto compare = get(i, j);
            if (compare != '.')
            {
                run(compare, i, j);
                run2(compare, i, j);
            }
        }
    }

    int unique = 0;
    int unique2 = 0;
    for (const auto [c1, c2] : blt::in_pairs(antinodes, antinodes2))
    {
        if (c1 == '#')
            unique++;
        if (c2 == '#')
            unique2++;
    }

    BLT_TRACE("Total Unique %d", unique);
    BLT_TRACE("Total Unique2 %d", unique2);
}
