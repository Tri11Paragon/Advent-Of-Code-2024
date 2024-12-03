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
#include <day2.h>
#include <vector>
#include <limits>
#include <algorithm>
#include <blt/fs/loader.h>
#include <blt/iterator/zip.h>
#include <blt/std/hashmap.h>

enum class state_t
{
    INCREASING, DECREASING, SAME
};

state_t get_state(const int i, const int j)
{
    return i > j ? state_t::INCREASING : (i < j ? state_t::DECREASING : state_t::SAME);
}

bool is_safe(const std::vector<int>& report)
{
    const state_t state = get_state(report[1], report[0]);
    bool is_safe = true;
    for (blt::size_t i = 0; i < report.size(); i++)
    {
        if (i + 1 >= report.size())
            break;
        const auto diff = std::abs(report[i] - report[i + 1]);
        const state_t new_state = report[i + 1] > report[i]
                                      ? state_t::INCREASING
                                      : (report[i + 1] < report[i] ? state_t::DECREASING : state_t::SAME);
        if (new_state != state)
        {
            is_safe = false;
            break;
        }
        if (diff < 1 || diff > 3)
        {
            is_safe = false;
            break;
        }
    }
    if (state != state_t::SAME && is_safe)
        return true;
    return false;
}

void run_day2()
{
    std::vector<std::vector<int>> reports;
    auto lines = blt::fs::getLinesFromFile("../problems/day2/input");

    for (const auto& line : lines)
    {
        const auto pair = blt::string::split(line, ' ');
        reports.emplace_back();
        for (const auto& var : pair)
            reports.back().push_back(std::stoi(var));
    }

    {
        int safe = 0;

        for (const auto& report : reports)
        {
            if (is_safe(report))
                safe++;
        }

        BLT_TRACE("Total Safe %d", safe);
    }

    {
        int safe = 0;

        for (const auto& report : reports)
        {
            if (is_safe(report))
                safe++;
            else
            {
                for (blt::size_t i = 0; i < report.size(); i++)
                {
                    auto copy = report;
                    copy.erase(copy.begin() + static_cast<blt::ptrdiff_t>(i));
                    if (is_safe(copy))
                    {
                        safe++;
                        break;
                    }
                }
            }
        }

        BLT_TRACE("Total Safe2 %d", safe);
    }
}
