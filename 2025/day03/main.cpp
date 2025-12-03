#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <ranges>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 03 2025" << std::endl;
    std::cout << "Lobby" << std::endl;
    std::cout << "Usage: day03_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

using Battery = std::vector<unsigned>;

// Parsing input
std::vector<Battery> parse_battery(std::ifstream& i_file)
{
    std::string line;
    std::vector<Battery> lines;
    while (std::getline(i_file, line))
    {
        Battery row;
        std::ranges::transform(line, std::back_inserter(row), [](char digit){ return static_cast<unsigned>(digit - '0'); });
        if (!row.empty()) lines.push_back(row);
    }
    return lines;
}

size_t joltage(const Battery& i_enabled_battery)
{
    return std::ranges::fold_left(
        i_enabled_battery,
        size_t{0},
        [](size_t acc, unsigned digit){ return acc * 10 + digit; }
    );
}

size_t joltage_n_digits(const Battery& i_battery, unsigned i_n_digits)
{
    if (i_n_digits >= i_battery.size())
        return joltage(i_battery);

    Battery enabled_battery(i_n_digits);
    auto it = i_battery.begin();
    auto enabled_it = enabled_battery.begin();
    for (auto i = 0; i < i_n_digits; ++i)
    {
        it = std::max_element(it, std::prev(i_battery.end(), i_n_digits - i - 1));
        *enabled_it = *it;
        it++;
        enabled_it++;
    }

    return joltage(enabled_battery);
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<Battery> batteries = parse_battery(file);
    size_t sum = std::accumulate(
        batteries.begin(),
        batteries.end(),
        size_t{0},
        [&](size_t acc, const auto& battery){ return acc + joltage_n_digits(battery, 2); }
    );
    std::cout << "Total output joltage: " << sum << std::endl;
    return 0;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<Battery> batteries = parse_battery(file);
    size_t sum = std::accumulate(
        batteries.begin(),
        batteries.end(),
        size_t{0},
        [&](size_t acc, const auto& battery){ return acc + joltage_n_digits(battery, 12); }
    );
    std::cout << "Total output joltage: " << sum << std::endl;
    return 0;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        help();
        return 1;
    }

    int retcode;

    try
    {
        std::cout << HEADER_DELIM << " 1st puzzle " << HEADER_DELIM << std::endl;
        retcode = main_puzzle1(argc, argv);

        std::cout << HEADER_DELIM << " 2nd puzzle " << HEADER_DELIM << std::endl;
        retcode += main_puzzle2(argc, argv);
    }
    catch(const std::exception& except)
    {
        std::cerr << "Exception caught: " << except.what() << std::endl;
        return 2;
    }
    catch(...)
    {
        std::cerr << "Unknown exception caught: " << std::endl;
        return 3;
    }

    return retcode;
}
