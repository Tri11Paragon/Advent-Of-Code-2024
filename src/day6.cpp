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
#include <day6.h>
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


namespace day6
{
    std::vector<std::vector<char>> map;

    blt::vec2i guard_pos;

    int width, height;

    int print_map()
    {
        int unique = 0;
        for (const auto& line : map)
        {
            for (const auto c : line)
            {
                BLT_TRACE_STREAM << c;
                if (c == 'X' || c == '+' || c == '-' || c == '=' || c == '|' || c == '!')
                    unique++;
            }
            BLT_TRACE_STREAM << "\n";
        }
        return unique;
    }
}

bool loops()
{

}

void run_day6()
{
    using namespace day6;
    auto file = blt::fs::getLinesFromFile("../problems/day6/input");

    for (const auto& line : file)
    {
        map.emplace_back();
        for (auto c : line)
            map.back().emplace_back(c);
    }

    height = static_cast<int>(map.size());
    width = static_cast<int>(map.begin()->size());
    BLT_TRACE("Width %d Height %d", width, height);

    for (auto [i, line] : blt::enumerate(map))
    {
        for (auto [j, c] : blt::enumerate(line))
        {
            if (c == '^')
            {
                guard_pos = {j, i};
                BLT_TRACE(guard_pos);
                goto out;
            }
        }
    }
out:

    while (true)
    {
        const char guard = map[guard_pos.y()][guard_pos.x()];
        map[guard_pos.y()][guard_pos.x()] = 'X';
        switch (guard)
        {
        case '^':
            if (guard_pos.y() - 1 < 0)
                goto exit;
            if (map[guard_pos.y() - 1][guard_pos.x()] != '#')
            {
                map[guard_pos.y()][guard_pos.x()] = '|';
                guard_pos -= blt::vec2i{0, 1};
                map[guard_pos.y()][guard_pos.x()] = '^';
            }else
            {
                map[guard_pos.y()][guard_pos.x()] = '+';
                guard_pos += blt::vec2i{1, 0};
                map[guard_pos.y()][guard_pos.x()] = '>';
            }
            break;
        case '>':
            if (guard_pos.x() + 1 >= width)
                goto exit;
            if (map[guard_pos.y()][guard_pos.x() + 1] != '#')
            {
                map[guard_pos.y()][guard_pos.x()] = '-';
                guard_pos += blt::vec2i{1, 0};
                map[guard_pos.y()][guard_pos.x()] = '>';
            }else
            {
                map[guard_pos.y()][guard_pos.x()] = '+';
                guard_pos += blt::vec2i{0, 1};
                map[guard_pos.y()][guard_pos.x()] = 'v';
            }
            break;
        case 'v':
            if (guard_pos.y() + 1 >= height)
                goto exit;
            if (map[guard_pos.y() + 1][guard_pos.x()] != '#')
            {
                map[guard_pos.y()][guard_pos.x()] = '!';
                guard_pos += blt::vec2i{0, 1};
                map[guard_pos.y()][guard_pos.x()] = 'v';
            }else
            {
                map[guard_pos.y()][guard_pos.x()] = '+';
                guard_pos -= blt::vec2i{1, 0};
                map[guard_pos.y()][guard_pos.x()] = '<';
            }
            break;
        case '<':
            if (guard_pos.x() - 1 < 0)
                goto exit;
            if (map[guard_pos.y()][guard_pos.x() - 1] != '#')
            {
                map[guard_pos.y()][guard_pos.x()] = '=';
                guard_pos -= blt::vec2i{1, 0};
                map[guard_pos.y()][guard_pos.x()] = '<';
            }else
            {
                map[guard_pos.y()][guard_pos.x()] = '+';
                guard_pos -= blt::vec2i{0, 1};
                map[guard_pos.y()][guard_pos.x()] = '^';
            }
            break;
        default:
            BLT_ERROR("Hey this shouldn't be possible! %c", guard);
            BLT_TRACE(print_map());
            std::exit(-1);
            break;
        }
    }
exit:

    const auto unique = print_map();

    BLT_TRACE("Unique Pos %d", unique);

}
