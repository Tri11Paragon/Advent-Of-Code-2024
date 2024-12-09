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
#include <vector>
#include <limits>
#include <algorithm>
#include <blt/fs/loader.h>
#include <blt/iterator/enumerate.h>
#include <blt/iterator/zip.h>
#include <blt/std/hashmap.h>

std::vector<char> chars;
const std::string pattern = "XMAS";
const std::string reverse = "SAMX";
const std::string mas = "MAS";
const std::string mas_r = "SAM";

int width, height;

char& get(int x, int y)
{
    return chars[y * width + x];
}

bool matches(int x, int y, char c)
{
    if (x >= width || y >= height)
        return false;
    if (x < 0 || y < 0)
        return false;
    return get(x, y) == c;
}

int search_at(int x, int y)
{
    int count = 0;

    bool correct = true;
    for (int i = 1; i < pattern.size(); i++)
    {
        if (!matches(x + i, y, pattern[i]))
        {
            correct = false;
            break;
        }
    }
    if (correct)
        count++;

    correct = true;
    for (int i = 1; i < pattern.size(); i++)
    {
        if (!matches(x - i, y, pattern[i]))
        {
            correct = false;
            break;
        }
    }
    if (correct)
        count++;

    correct = true;
    for (int i = 1; i < pattern.size(); i++)
    {
        if (!matches(x, y + i, pattern[i]))
        {
            correct = false;
            break;
        }
    }
    if (correct)
        count++;

    correct = true;
    for (int i = 1; i < pattern.size(); i++)
    {
        if (!matches(x, y - i, pattern[i]))
        {
            correct = false;
            break;
        }
    }
    if (correct)
        count++;

    correct = true;
    for (int i = 1; i < pattern.size(); i++)
    {
        if (!matches(x + i, y + i, pattern[i]))
        {
            correct = false;
            break;
        }
    }
    if (correct)
        count++;

    correct = true;
    for (int i = 1; i < pattern.size(); i++)
    {
        if (!matches(x + i, y - i, pattern[i]))
        {
            correct = false;
            break;
        }
    }
    if (correct)
        count++;

    correct = true;
    for (int i = 1; i < pattern.size(); i++)
    {
        if (!matches(x - i, y + i, pattern[i]))
        {
            correct = false;
            break;
        }
    }
    if (correct)
        count++;

    correct = true;
    for (int i = 1; i < pattern.size(); i++)
    {
        if (!matches(x - i, y - i, pattern[i]))
        {
            correct = false;
            break;
        }
    }
    if (correct)
        count++;

    return count;
}

bool has(int x, int y, int dirx, int diry)
{
    for (int i = 0; i < mas.size(); i++)
    {
        if (!matches(x + (i * dirx), y + (i * diry), mas[i]))
            return false;
    }
    return true;
}

bool has_r(int x, int y, int dirx, int diry)
{
    for (int i = 0; i < mas.size(); i++)
    {
        if (!matches(x + (i * dirx), y + (i * diry), mas_r[i]))
            return false;
    }
    return true;
}

int x_mas(int x, int y)
{
    return (has(x - 1, y - 1, 1, 1) || has_r(x - 1, y - 1, 1, 1)) && (has(x - 1, y + 1, 1, -1) || has_r(x - 1, y + 1, 1, -1));
}

void run_day4()
{
    auto file = blt::fs::getLinesFromFile("../problems/day4/input");

    width = static_cast<int>(file.size()), height = static_cast<int>(file.begin()->size());

    chars.resize(width * height);
    for (const auto& [i, line] : blt::enumerate(file))
    {
        for (const auto [j, c] : blt::enumerate(line))
        {
            get(j, i) = static_cast<char>(std::toupper(c));
        }
    }

    BLT_TRACE("Width %d Height %d", width, height);

    int count = 0;
    int count2 = 0;

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            if (matches(i, j, 'X'))
                count += search_at(i, j);
            if (matches(i, j, 'A'))
                count2 += x_mas(i, j);
        }
    }

    BLT_TRACE("XMASes %d", count);
    BLT_TRACE("X-MASes %d", count2);
}
