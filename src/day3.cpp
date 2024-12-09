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
#include <variant>

enum class state_t
{
    NONE, M, U, L, D, O, N, AP, T, OPEN, NUM1, NUM2
};

void run_day3()
{
    auto file = blt::fs::getFile("../problems/day3/input");

    {
        auto CURRENT_STATE = state_t::NONE;

        int total = 0;

        std::string num1, num2;

        for (auto [i, c] : blt::enumerate(file))
        {
            switch (CURRENT_STATE)
            {
            case state_t::NONE:
                if (c == 'm')
                    CURRENT_STATE = state_t::M;
                break;
            case state_t::M:
                if (c == 'u')
                    CURRENT_STATE = state_t::U;
                else
                    CURRENT_STATE = state_t::NONE;
                break;
            case state_t::U:
                if (c == 'l')
                    CURRENT_STATE = state_t::L;
                else
                    CURRENT_STATE = state_t::NONE;
                break;
            case state_t::L:
                if (c == '(')
                    CURRENT_STATE = state_t::NUM1;
                else
                    CURRENT_STATE = state_t::NONE;
                break;
            case state_t::NUM1:
                if (std::isdigit(c))
                    num1 += c;
                else if (c == ',')
                    CURRENT_STATE = state_t::NUM2;
                else
                {
                    num1 = {};
                    CURRENT_STATE = state_t::NONE;
                }
                break;
            case state_t::NUM2:
                if (std::isdigit(c))
                    num2 += c;
                else
                {
                    if (c == ')' && !(num1.empty() || num2.empty()))
                        total += std::stoi(num1) * std::stoi(num2);
                    num1 = {};
                    num2 = {};
                    CURRENT_STATE = state_t::NONE;
                }
                break;
            default:
                break;
            }
        }

        BLT_TRACE("Total value %d", total);
    }

    auto CURRENT_STATE = state_t::NONE;

    int total = 0;
    bool run = true;
    bool detecting = false;

    std::string num1, num2;

    for (const auto c : file)
    {
        switch (CURRENT_STATE)
        {
        case state_t::NONE:
            if (c == 'm')
                CURRENT_STATE = state_t::M;
            else if (c == 'd')
                CURRENT_STATE = state_t::D;
            break;
        case state_t::M:
            if (c == 'u')
                CURRENT_STATE = state_t::U;
            else
                CURRENT_STATE = state_t::NONE;
            break;
        case state_t::U:
            if (c == 'l')
                CURRENT_STATE = state_t::L;
            else
                CURRENT_STATE = state_t::NONE;
            break;
        case state_t::L:
            if (c == '(')
                CURRENT_STATE = state_t::NUM1;
            else
                CURRENT_STATE = state_t::NONE;
            break;
        case state_t::NUM1:
            if (std::isdigit(c))
                num1 += c;
            else if (c == ',')
                CURRENT_STATE = state_t::NUM2;
            else
            {
                num1 = {};
                CURRENT_STATE = state_t::NONE;
            }
            break;
        case state_t::NUM2:
            if (std::isdigit(c))
                num2 += c;
            else
            {
                if (c == ')' && run && !(num1.empty() || num2.empty()))
                    total += std::stoi(num1) * std::stoi(num2);
                num1 = {};
                num2 = {};
                CURRENT_STATE = state_t::NONE;
            }
            break;
        case state_t::D:
            if (c == 'o')
                CURRENT_STATE = state_t::O;
            else if (c == 'm')
                CURRENT_STATE = state_t::M;
            else
                CURRENT_STATE = state_t::NONE;
            break;
        case state_t::O:
            if (c == '(')
            {
                CURRENT_STATE = state_t::OPEN;
                detecting = true;
            }
            else if (c == 'n')
            {
                CURRENT_STATE = state_t::N;
                detecting = false;
            }
            else if (c == 'm')
                CURRENT_STATE = state_t::M;
            else
                CURRENT_STATE = state_t::NONE;
            break;
        case state_t::N:
            if (c == '\'')
                CURRENT_STATE = state_t::AP;
            else if (c == 'm')
                CURRENT_STATE = state_t::M;
            else
                CURRENT_STATE = state_t::NONE;
            break;
        case state_t::AP:
            if (c == 't')
                CURRENT_STATE = state_t::T;
            else if (c == 'm')
                CURRENT_STATE = state_t::M;
            else
                CURRENT_STATE = state_t::NONE;
            break;
        case state_t::T:
            if (c == '(')
                CURRENT_STATE = state_t::OPEN;
            else if (c == 'm')
                CURRENT_STATE = state_t::M;
            else
                CURRENT_STATE = state_t::NONE;
            break;
        case state_t::OPEN:
            if (c == ')')
            {
                if (detecting)
                    run = true;
                else
                    run = false;
                CURRENT_STATE = state_t::NONE;
            } else if (c == 'm')
                CURRENT_STATE = state_t::M;
            else
                CURRENT_STATE = state_t::NONE;
            break;
        }
    }

    BLT_TRACE("Total value2 %d", total);
}
