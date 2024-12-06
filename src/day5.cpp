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
#include <day5.h>
#include <vector>
#include <limits>
#include <algorithm>
#include <blt/fs/loader.h>
#include <blt/iterator/enumerate.h>
#include <blt/iterator/iterator.h>
#include <blt/iterator/zip.h>
#include <blt/std/hashmap.h>

blt::hashmap_t<int, blt::hashset_t<int>> after;
std::vector<std::vector<int>> page_updates;

std::vector<std::vector<int>> correct_pages;
std::vector<std::vector<int>> incorrect_pages;

bool is_correct(const std::vector<int>& page)
{
    for (const auto& [i, v] : blt::enumerate(page))
    {
        auto cant_be_after = after[v];
        for (auto v2 : blt::iterate(page).skip(i))
        {
            if (cant_be_after.contains(v2))
                return false;
        }
    }
    return true;
}

void correct(std::vector<int>& page)
{
    std::vector<int> copy = page;
    for (const auto& [i, v] : blt::enumerate(page))
    {
        auto cant_be_after = after[v];
        for (auto v2 : blt::iterate(page).skip(i))
        {
            if (cant_be_after.contains(v2))
            {
                copy.erase(std::find(copy.begin(), copy.end(), v2));
                copy.insert(copy.begin() + static_cast<blt::ptrdiff_t>(i), v2);
            }
        }
    }
    // for (const auto& v : copy)
        // std::cout << v << ", ";
    // std::cout << "\n";
    page = copy;
}

void run_day5()
{
    auto file = blt::fs::getLinesFromFile("../problems/day5/input");

    auto it = file.begin();
    while (!it->empty())
    {
        if (blt::string::contains(*it, ','))
            break;
        auto line = blt::string::split(*it, '|');
        after[std::stoi(line[1])].insert(std::stoi(line[0]));
        ++it;
    }

    for (; it != file.end(); ++it)
    {
        auto line = blt::string::split(*it, ',');
        page_updates.emplace_back();
        for (const auto& v : line)
            page_updates.back().emplace_back(std::stoi(v));
    }

    for (const auto& pages : page_updates)
    {
        if (is_correct(pages))
            correct_pages.push_back(pages);
        else
            incorrect_pages.push_back(pages);
    }

    BLT_TRACE("Correct pages %ld", correct_pages.size());

    int count = 0;

    for (const auto& page : correct_pages)
    {
        count += page[(page.size() / 2)];
    }

    int count2 = 0;

    for (auto& page : incorrect_pages)
    {
        for (int i = 0; i < page.size(); i++)
            correct(page);
        BLT_TRACE(is_correct(page) ? "True" : "False");
        count2 += page[(page.size() / 2)];
    }


    BLT_TRACE("Total midpoint correct %d", count);
    BLT_TRACE("Total midpoint correct (corrected) %d", count2);
}
