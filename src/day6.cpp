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

template <>
struct std::hash<blt::vec2i>
{
    std::size_t operator()(const blt::vec2i& s) const noexcept
    {
        blt::size_t seed = 0x5F2D0AB0;
        seed ^= (seed << 6) + (seed >> 2) + 0xCDA44F00 + std::hash<blt::i32>{}(s.x());
        seed ^= (seed << 6) + (seed >> 2) + 0xF1284DE9 + std::hash<blt::i32>{}(s.y());
        return seed;
    }
};

namespace day6
{
    int width, height;

    int print_map(const std::vector<std::vector<char>>& map)
    {
        int unique = 0;
        for (const auto& line : map)
        {
            for (const auto c : line)
            {
                BLT_TRACE_STREAM << c;
                if (c == 'X')
                    unique++;
            }
            BLT_TRACE_STREAM << "\n";
        }
        return unique;
    }

    blt::vec2i find_guard_pos(const std::vector<std::vector<char>>& map)
    {
        for (auto [i, line] : blt::enumerate(map))
        {
            for (auto [j, c] : blt::enumerate(line))
            {
                if (c == '^' || c == '>' || c == '<' || c == 'v')
                    return {j, i};
            }
        }
        BLT_ABORT("Unable to find guard!");
    }

    struct point
    {
        blt::vec2i pos;
        char dir;

        point(const blt::vec2i& pos, const char dir): pos(pos), dir(dir)
        {
        }

        friend std::size_t hash_value(const point& obj)
        {
            std::size_t seed = 0x5410391E;
            seed ^= (seed << 6) + (seed >> 2) + 0x0CDB4CE0 + std::hash<blt::vec2i>{}(obj.pos);
            seed ^= (seed << 6) + (seed >> 2) + 0x3923C237 + static_cast<std::size_t>(obj.dir);
            return seed;
        }

        friend bool operator==(const point& lhs, const point& rhs)
        {
            return lhs.pos == rhs.pos
                && lhs.dir == rhs.dir;
        }

        friend bool operator!=(const point& lhs, const point& rhs)
        {
            return !(lhs == rhs);
        }
    };

    bool execute_step(const char guard, std::vector<std::vector<char>>& map, blt::vec2i& guard_pos)
    {
        switch (guard)
        {
        case '^':
            if (guard_pos.y() - 1 < 0)
                return false;
            if (map[guard_pos.y() - 1][guard_pos.x()] != '#')
            {
                guard_pos -= blt::vec2i{0, 1};
                map[guard_pos.y()][guard_pos.x()] = '^';
            }
            else
            {
                // guard_pos += blt::vec2i{1, 0};
                map[guard_pos.y()][guard_pos.x()] = '>';
            }
            break;
        case '>':
            if (guard_pos.x() + 1 >= day6::width)
                return false;
            if (map[guard_pos.y()][guard_pos.x() + 1] != '#')
            {
                guard_pos += blt::vec2i{1, 0};
                map[guard_pos.y()][guard_pos.x()] = '>';
            }
            else
            {
                // guard_pos += blt::vec2i{0, 1};
                map[guard_pos.y()][guard_pos.x()] = 'v';
            }
            break;
        case 'v':
            if (guard_pos.y() + 1 >= day6::height)
                return false;
            if (map[guard_pos.y() + 1][guard_pos.x()] != '#')
            {
                guard_pos += blt::vec2i{0, 1};
                map[guard_pos.y()][guard_pos.x()] = 'v';
            }
            else
            {
                // guard_pos -= blt::vec2i{1, 0};
                map[guard_pos.y()][guard_pos.x()] = '<';
            }
            break;
        case '<':
            if (guard_pos.x() - 1 < 0)
                return false;
            if (map[guard_pos.y()][guard_pos.x() - 1] != '#')
            {
                guard_pos -= blt::vec2i{1, 0};
                map[guard_pos.y()][guard_pos.x()] = '<';
            }
            else
            {
                // guard_pos -= blt::vec2i{0, 1};
                map[guard_pos.y()][guard_pos.x()] = '^';
            }
            break;
        default:
            BLT_ERROR("Hey this shouldn't be possible! %c", guard);
            BLT_TRACE(day6::print_map(map));
            std::exit(-1);
        }
        return true;
    }

    bool loops(std::vector<std::vector<char>>& map)
    {
        blt::hashset_t<point> paths;
        blt::vec2i guard_pos = find_guard_pos(map);

        while (true)
        {
            const char guard = map[guard_pos.y()][guard_pos.x()];
            if (paths.contains({guard_pos, guard}))
                return true;
            paths.insert({guard_pos, guard});
            map[guard_pos.y()][guard_pos.x()] = 'X';
            if (!execute_step(guard, map, guard_pos))
                return false;
        }
    }

    void run_part1(std::vector<std::vector<char>> map)
    {
        blt::vec2i guard_pos = find_guard_pos(map);

        while (true)
        {
            const char guard = map[guard_pos.y()][guard_pos.x()];
            map[guard_pos.y()][guard_pos.x()] = 'X';
            if (!execute_step(guard, map, guard_pos))
                goto exit;
        }
    exit:

        const auto unique = print_map(map);

        BLT_TRACE("Unique Pos %d", unique);
    }

    void run_part2(const std::vector<std::vector<char>>& map)
    {
        int loopy = 0;
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                if (map[j][i] != '^' && map[j][i] != '#')
                {
                    auto copy = map;
                    copy[j][i] = '#';
                    if (loops(copy))
                    {
                        loopy++;
                        BLT_TRACE("found a loopy at %d %d", j, i);
                        // print_map(copy);
                    }
                }
            }
        }
        BLT_TRACE("Loopy %d", loopy);
    }
}

void run_day6()
{
    using namespace day6;

    std::vector<std::vector<char>> map;

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

    run_part1(map);
    run_part2(map);
}
