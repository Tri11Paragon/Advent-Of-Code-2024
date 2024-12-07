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
#include <day7.h>
#include <blt/math/log_util.h>
#include <vector>
#include <limits>
#include <algorithm>
#include <stack>
#include <blt/fs/loader.h>
#include <blt/iterator/enumerate.h>
#include <blt/iterator/iterator.h>
#include <blt/iterator/zip.h>
#include <blt/std/hashmap.h>
#include <blt/math/vectors.h>
#include <blt/std/assert.h>

struct test_case_t
{
    blt::i64 calibration;
    std::vector<blt::i64> values;
};

enum class operator_t
{
    PLUS, MULTIPLY, CONCAT
};

struct operation_t
{
    const test_case_t* test_case;
    std::vector<operator_t> operators;

    explicit operation_t(const test_case_t* test_case): test_case(test_case)
    {
        for (blt::size_t i = 0; i < test_case->values.size() - 1; ++i)
            operators.push_back(operator_t::PLUS);
    }

    [[nodiscard]] bool valid() const
    {
        std::stack<blt::i64> value_stack;

        for (const auto v : blt::iterate(test_case->values).rev())
            value_stack.push(v);

        for (const auto op : operators)
        {
            const auto a = value_stack.top();
            value_stack.pop();
            const auto b = value_stack.top();
            value_stack.pop();

            switch (op)
            {
            case operator_t::PLUS:
                value_stack.push(a + b);
                break;
            case operator_t::MULTIPLY:
                value_stack.push(a * b);
                break;
            case operator_t::CONCAT:
                value_stack.push(std::stoll(std::to_string(a) + std::to_string(b)));
                break;
            }
        }

        return test_case->calibration == value_stack.top();
    }

    bool next()
    {
        bool carry = false;
        for (blt::size_t i = 0; i < operators.size(); i++)
        {
            if (carry && operators[i] == operator_t::MULTIPLY)
            {
                if (i == operators.size() - 1)
                    return false;
                operators[i] = operator_t::PLUS;
            }
            else if (carry && operators[i] == operator_t::PLUS)
            {
                operators[i] = operator_t::MULTIPLY;
                break;
            }
            else if (operators[i] == operator_t::PLUS)
            {
                operators[i] = operator_t::MULTIPLY;
                break;
            }
            else if (operators[i] == operator_t::MULTIPLY)
            {
                if (i == operators.size() - 1)
                    return false;
                carry = true;
                operators[i] = operator_t::PLUS;
            }
            else
            {
                BLT_INFO("I don't think this state is possible");
            }
        }
        return true;
    }

    bool next2()
    {
        bool carry = false;
        for (blt::size_t i = 0; i < operators.size(); i++)
        {
            if (carry && operators[i] == operator_t::CONCAT)
            {
                if (i == operators.size() - 1)
                    return false;
                operators[i] = operator_t::PLUS;
            }
            else if (operators[i] == operator_t::PLUS)
            {
                operators[i] = operator_t::MULTIPLY;
                break;
            }
            else if (operators[i] == operator_t::MULTIPLY)
            {
                operators[i] = operator_t::CONCAT;
                break;
            }
            else if (operators[i] == operator_t::CONCAT)
            {
                if (i == operators.size() - 1)
                    return false;
                carry = true;
                operators[i] = operator_t::PLUS;
            }
            else
            {
                BLT_INFO("I don't think this state is possible");
            }
        }
        return true;
    }
};

void run_day7()
{
    const auto file = blt::fs::getLinesFromFile("../problems/day7/input");

    std::vector<test_case_t> test_cases;

    for (const auto& line : file)
    {
        test_case_t test;
        auto data = blt::string::split(line, ":");
        test.calibration = std::stoll(data[0]);
        auto values = blt::string::split(data[1], ' ');
        for (const auto& value : values)
            test.values.push_back(std::stoll(value));
        test_cases.emplace_back(std::move(test));
    }

    blt::i64 valid_test_case_sum = 0;

    for (const auto& test : test_cases)
    {
        operation_t op{&test};
        do
        {
            if (op.valid())
            {
                BLT_TRACE("Valid test case found! %ld", test.calibration);
                valid_test_case_sum += test.calibration;
                break;
            }
        }
        while (op.next());
    }

    blt::i64 valid_test_cases_sum2 = 0;

    for (const auto& test : test_cases)
    {
        operation_t op{&test};
        do
        {
            if (op.valid())
            {
                BLT_TRACE("Valid test case found! %ld", test.calibration);
                valid_test_cases_sum2 += test.calibration;
                break;
            }
        }
        while (op.next2());
    }

    BLT_INFO("test_case_sum %ld", valid_test_case_sum);
    BLT_INFO("test_case_sum2 %ld", valid_test_cases_sum2);
}
