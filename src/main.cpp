#include <blt/parse/argparse.h>
#include <blt/std/logging.h>
#include <days.h>

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
    case 9:
        run_day9();
        break;
    case 10:
        run_day10();
        break;
    case 11:
        run_day11();
        break;
    case 12:
        run_day12();
        break;
    case 13:
        run_day13();
        break;
    case 14:
        run_day14();
        break;
    case 15:
        run_day15();
        break;
    case 16:
        run_day16();
        break;
    case 17:
        run_day17();
        break;
    case 18:
        run_day18();
        break;
    case 19:
        run_day19();
        break;
    case 20:
        run_day20();
        break;
    case 21:
        run_day21();
        break;
    case 22:
        run_day22();
        break;
    case 23:
        run_day23();
        break;
    case 24:
        run_day24();
        break;
    default:
        BLT_INFO("%d is not a day", day);
        break;
    }
}
