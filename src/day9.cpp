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

struct data_t
{
    int id;
    int size;
};

struct free_space_t
{
    int size;
};

using var_v = std::variant<data_t, free_space_t>;

void print(std::vector<var_v>& vec)
{
    for (const auto& v : vec)
    {
        std::visit(blt::lambda_visitor{
                       [](const data_t& data)
                       {
                           for (int i = 0; i < data.size; i++)
                               std::cout << data.id;
                       },
                       [](const free_space_t& free)
                       {
                           for (int i = 0; i < free.size; i++)
                               std::cout << '.';
                       }
                   }, v);
    }
    std::cout << '\n';
}

blt::size_t check_sum(const std::vector<var_v>& processed_file)
{
    blt::size_t total = 0;
    blt::size_t position = 0;
    for (auto& i : processed_file)
    {
        blt::lambda_visitor visit{
            [&total, &position](const data_t& data)
            {
                for (int j = 0; j < data.size; j++)
                {
                    total += data.id * position++;
                }
            },
            [&total, &position](const free_space_t& free)
            {
                for (int j = 0; j < free.size; j++)
                    position++;
            }
        };
        std::visit(visit, i);
    }
    return total;
}

void part1(std::vector<var_v> processed_file)
{
    while (true)
    {
        auto& top = processed_file.back();
        if (std::holds_alternative<free_space_t>(top))
        {
            processed_file.pop_back();
            continue;
        }
        auto& data = std::get<data_t>(top);
        bool found = false;
        for (auto it = processed_file.begin(); it != processed_file.end(); ++it)
        {
            if (std::holds_alternative<free_space_t>(*it))
            {
                auto& place = std::get<free_space_t>(*it);
                auto remain = static_cast<blt::ptrdiff_t>(place.size) - static_cast<blt::ptrdiff_t>(data.size);
                // BLT_TRACE("id: %d;; %d - %d = %ld", data.id, data.size, place.size, remain);
                if (remain < 0)
                {
                    data.size -= place.size;
                    processed_file.insert(processed_file.erase(it), data_t{data.id, place.size});
                }
                else if (remain > 0)
                {
                    place.size -= data.size;
                    processed_file.insert(it, data_t{data.id, data.size});
                    processed_file.pop_back();
                }
                else
                {
                    processed_file.insert(processed_file.erase(it), data);
                    processed_file.pop_back();
                }
                found = true;
                break;
            }
        }

        if (!found)
            break;
    }


    std::cout << "Checksum " << check_sum(processed_file) << " " << processed_file.size() << "\n";
}

void part2(std::vector<var_v> processed_file)
{
    int last_id = std::numeric_limits<int>::max();
    do
    {
        for (const auto& val : blt::iterate(processed_file).rev())
        {
            if (std::holds_alternative<data_t>(val))
            {
                if (const auto id = std::get<data_t>(val).id; id < last_id)
                {
                    last_id = id;
                    break;
                }
            }
        }
        auto dv = processed_file.end();
        do
        {
            --dv;
            if (std::holds_alternative<data_t>(*dv))
            {
                if (std::get<data_t>(*dv).id == last_id)
                    break;
            }
        }
        while (true);

        auto data = std::get<data_t>(*dv);

        bool found = false;
        for (auto fs = processed_file.begin(); fs != processed_file.end(); ++fs)
        {
            // no free space left
            if (last_id == 0 && fs == dv)
                goto exit;
            if (std::holds_alternative<free_space_t>(*fs))
            {
                auto& space = std::get<free_space_t>(*fs);

                if (data.size < space.size)
                {
                    space.size -= data.size;
                    processed_file.insert(fs, data);
                    found = true;
                    break;
                }
                if (data.size == space.size)
                {
                    processed_file.insert(processed_file.erase(fs), data);
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            for (auto it = processed_file.end() - 1; it != processed_file.begin() - 1; --it)
            {
                if (std::holds_alternative<data_t>(*it))
                {
                    if (std::get<data_t>(*it).id == last_id)
                    {
                        processed_file.insert(processed_file.erase(it), free_space_t{data.size});
                        break;
                    }
                }
            }
        }
    }
    while (true);
exit:

    // print(processed_file);

    std::cout << "Checksum2 " << check_sum(processed_file) << " " << processed_file.size() << "\n";
}

void run_day9()
{
    const auto file = blt::fs::getFile("../problems/day9/input");

    std::vector<var_v> processed_file;
    int id = 0;

    for (blt::size_t i = 0; i < file.size(); i++)
    {
        const auto blocks = file[i] - '0';
        processed_file.emplace_back(data_t{id, blocks});
        id++;
        if (i < file.size() - 1)
        {
            i++;
            const auto free = file[i] - '0';
            processed_file.emplace_back(free_space_t{free});
        }
    }

    BLT_TRACE(processed_file.size());
    // print(processed_file);

    part1(processed_file);
    part2(processed_file);
}
