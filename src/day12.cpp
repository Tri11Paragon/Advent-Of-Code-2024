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
#include <queue>
#include <blt/fs/loader.h>
#include <blt/iterator/enumerate.h>
#include <blt/iterator/iterator.h>
#include <blt/iterator/zip.h>
#include <blt/std/hashmap.h>
#include <blt/math/vectors.h>
#include <blt/std/assert.h>

namespace day12
{
    std::vector<std::vector<char>> plots;
    std::vector<std::vector<int>> perimeters;
    std::vector<std::vector<int>> params;
    std::vector<blt::vec2i> founds;
    blt::hashset_t<blt::vec2i> visited;
    blt::hashset_t<blt::vec2i> visited_side;

    int width, height;

    template <typename T>
    std::optional<blt::ref<std::remove_reference_t<decltype(std::declval<T>()[0][0])>>> get(T& t, const int x, const int y)
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
            return {};
        return t[y][x];
    }

    blt::vec2i floodfill(const char prev, const int x, const int y)
    {
        if (visited.contains({x, y}) || x < 0 || y < 0 || x >= width || y >= height)
            return {0, 0};
        const auto cur = plots[y][x];
        if (cur != prev)
            return {0, 0};
        visited.insert({x, y});
        return blt::vec2i{1, perimeters[y][x]} + floodfill(cur, x + 1, y) + floodfill(cur, x - 1, y) + floodfill(cur, x, y + 1) +
            floodfill(cur, x, y - 1);
    }

    blt::vec2i flood(const int x, const int y)
    {
        return floodfill(plots[y][x], x, y);
    }

    template <typename T>
    void print(const T& t)
    {
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
                std::cout << t[j][i] << "";
            std::cout << std::endl;
        }
    }

    struct region
    {
        blt::vec2i start, end;
    };

    void expand(region& region, char prev)
    {
        for (const auto& v : {blt::vec2i{-1, 0}, blt::vec2i{0, -1}})
        {
            const auto n_pos = region.start + v;
            if (plots[n_pos.y()][n_pos.x()] == prev)
                region.start = n_pos;
        }
        for (const auto& v : {blt::vec2i{1, 0}, blt::vec2i{0, 1}})
        {
            const auto n_pos = region.end + v;
            if (plots[n_pos.y()][n_pos.x()] == prev)
                region.end = n_pos;
        }
    }
}

void run_day12()
{
    using namespace day12;
    auto file = blt::fs::getLinesFromFile("../problems/day12/example1");

    width = static_cast<int>(file.begin()->size());
    height = static_cast<int>(file.size());

    for (const auto& line : file)
    {
        plots.emplace_back();
        perimeters.emplace_back();
        for (const auto c : line)
        {
            plots.back().push_back(c);
            perimeters.back().push_back(-1);
        }
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int per = 0;
            const auto cur = plots[y][x];
            if (const auto v = get(plots, x + 1, y); !v || v->get() != cur)
                per++;
            if (const auto v = get(plots, x - 1, y); !v || v->get() != cur)
                per++;
            if (const auto v = get(plots, x, y + 1); !v || v->get() != cur)
                per++;
            if (const auto v = get(plots, x, y - 1); !v || v->get() != cur)
                per++;
            perimeters[y][x] = per;
        }
    }

    int total = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            auto fill = flood(x, y);
            const auto size = fill.x() * fill.y();
            total += size;
        }
    }

    BLT_TRACE(total);

    int total2 = 0;
    visited.clear();
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (visited_side.contains({x, y}))
                continue;
            auto cur = plots[y][x];
            int edges = 0;
            int area = flood(x, y).x();

            std::queue<blt::vec2i> pos_to_check;
            pos_to_check.push({x, y});

            while (!pos_to_check.empty())
            {
                auto pos = pos_to_check.front();
                pos_to_check.pop();

                if (visited_side.contains(pos))
                    continue;

                bool x_pos = false, x_neg = false, y_pos = false, y_neg = false;
                bool x_y_pos = false, x_y_neg = false, x_pos_y_neg = false, x_neg_y_pos = false;

                {
                    const auto v = get(plots, pos.x() + 1, pos.y());
                    x_pos = v && v->get() == cur;
                }
                {
                    const auto v = get(plots, pos.x() - 1, pos.y());
                    x_neg = v && v->get() == cur;
                }
                {
                    const auto v = get(plots, pos.x(), pos.y() + 1);
                    y_pos = v && v->get() == cur;
                }
                {
                    const auto v = get(plots, pos.x(), pos.y() - 1);
                    y_neg = v && v->get() == cur;
                }
                {
                    const auto v = get(plots, pos.x() + 1, pos.y() + 1);
                    x_y_pos = v && v->get() != cur;
                }
                {
                    const auto v = get(plots, pos.x() - 1, pos.y() - 1);
                    x_y_neg = v && v->get() != cur;
                }
                {
                    const auto v = get(plots, pos.x() + 1, pos.y() - 1);
                    x_pos_y_neg = v && v->get() != cur;
                }
                {
                    const auto v = get(plots, pos.x() - 1, pos.y() + 1);
                    x_neg_y_pos = v && v->get() != cur;
                }

                if (x_pos)
                    pos_to_check.push({pos.x() + 1, pos.y()});
                if (y_pos)
                    pos_to_check.push({pos.x(), pos.y() + 1});
                if (x_neg)
                    pos_to_check.push({pos.x() - 1, pos.y()});
                if (y_neg)
                    pos_to_check.push({pos.x(), pos.y() - 1});

                auto b4 = edges;
                if (x_pos && (y_pos && !y_neg))
                {
                    edges++;
                    if (x_y_pos)
                        edges++;
                }
                if (x_pos && (!y_pos && y_neg))
                {
                    edges++;
                    // if (x_pos_y_neg)
                    //     edges++;
                }
                if (y_neg && (x_pos && !x_neg))
                {
                    edges++;
                    // if (x_pos_y_neg)
                    //     edges++;
                }
                if (y_neg && (!x_pos && x_neg))
                {
                    edges++;
                    // if (x_y_neg)
                    //     edges++;
                }
                if (x_neg && (y_pos && !y_neg))
                {
                    edges++;
                    // if (x_neg_y_pos)
                    //     edges++;
                }
                if (x_neg && (!y_pos && y_neg))
                {
                    edges++;
                    // if (x_y_neg)
                    //     edges++;
                }
                if (y_pos && (x_pos && !x_neg))
                {
                    edges++;
                    if (x_y_pos)
                        edges++;
                }
                if (y_pos && (!x_pos && x_neg))
                {
                    edges++;
                    // if (x_neg_y_pos)
                    //     edges++;
                }

                if ((x_neg && !x_pos && !y_pos && !y_neg) || (!x_neg && x_pos && !y_pos && !y_neg))
                    edges += 2;
                if ((!x_neg && !x_pos && y_pos && !y_neg) || (!x_neg && !x_pos && !y_pos && y_neg))
                    edges += 2;
                if (!x_neg && !x_pos && !y_pos && !y_neg)
                    edges += 4;

                BLT_TRACE("Visit %c at %d %d edges %d (%d)", cur, pos.x(), pos.y(), edges, edges-b4);


                visited_side.insert(pos);
            }
            BLT_TRACE("For %c do %d edges with %d area", cur, edges, area);
            total2 += edges * area;
        }
    }
    BLT_TRACE(total2);
}
