#include <blt/parse/argparse.h>
#include <blt/std/logging.h>
#include <day1.h>
#include <day2.h>
#include <day3.h>
#include <day4.h>
#include <day5.h>
#include <day6.h>
#include <day7.h>
#include <day8.h>

int main(const int argc, const char** argv)
{
    blt::arg_parse parser;

    parser.addArgument(blt::arg_builder{"day"}.build());

    auto args = parser.parse_args(argc, argv);

    if (!args.contains("day"))
    {
        BLT_ERROR("Please provide the day to run");
        return -1;
    }

    switch (const auto day = std::stoi(args.get<std::string>("day")))
    {
    case 1:
        run_day1();
        break;
    case 2:
        run_day2();
        break;
    case 3:
        run_day3();
        break;
    case 4:
        run_day4();
        break;
    case 5:
        run_day5();
        break;
    case 6:
        run_day6();
        break;
    case 7:
        run_day7();
        break;
    case 8:
        run_day8();
        break;
    default:
        BLT_INFO("%d is not a day", day);
        break;
    }
}
