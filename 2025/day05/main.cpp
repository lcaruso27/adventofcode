#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <utility>
#include <tuple>
#include <iterator>
#include <ranges>
#include <algorithm>
#include <exception>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 05 2025" << std::endl;
    std::cout << "Cafeteria" << std::endl;
    std::cout << "Usage: day05_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}


using RangeInclusive = std::pair<size_t, size_t>;


// Parse ranges and fresh ids
std::tuple<std::vector<RangeInclusive>, std::vector<size_t>>
    parse_ranges_and_ids(std::ifstream& i_file)
{
    // 1st ranges
    std::vector<RangeInclusive> fresh_ranges;
    std::string line;
    while (std::getline(i_file, line))
    {
        if (line.empty()) break;
        auto pos = line.find('-');
        if (pos == std::string::npos) throw std::runtime_error("no '-' found in first part");
        fresh_ranges.emplace_back(std::stoul(line.substr(0, pos)), std::stoul(line.substr(pos + 1)));
    }
    // 2nd ids
    std::vector<size_t> ingredients{
        std::istream_iterator<size_t>{ i_file }, std::istream_iterator<size_t>()
    };

    return std::make_tuple(fresh_ranges, ingredients);
}

// Count fresh ids

bool is_fresh(const std::vector<RangeInclusive>& i_ranges, size_t i_ingredient)
{
    auto it_found = std::find_if(
        i_ranges.begin(),
        i_ranges.end(),
        [&i_ingredient](const auto& pair){ return pair.first <= i_ingredient && i_ingredient <= pair.second; }
    );
    return it_found != i_ranges.end();
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    auto [fresh_ranges, ingredients] = parse_ranges_and_ids(file);
    size_t n_fresh = std::ranges::count_if(ingredients, [&fresh_ranges](size_t ingredient){ return is_fresh(fresh_ranges, ingredient); });
    std::cout << "Number of fresh available ingredients: " << n_fresh << std::endl;
    return 0;
}

// Count all fresh ids

void merge_ranges(std::vector<RangeInclusive>& io_ranges)
{
    if (io_ranges.empty()) return;

    std::ranges::sort(io_ranges);
    auto it_reference = io_ranges.begin();
    auto it_candidate = std::next(io_ranges.begin());
    while (it_candidate != io_ranges.end())
    {
        // check if disjoint
        if (it_candidate->first > it_reference->second)
        {
            it_reference++;
            it_candidate++;
        }
        // are overlapping
        else
        {
            it_reference->first = std::min(it_reference->first, it_candidate->first);
            it_reference->second = std::max(it_reference->second, it_candidate->second);
            it_candidate = io_ranges.erase(it_candidate);
        }
    }
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<RangeInclusive> fresh_ranges;
    std::tie(fresh_ranges, std::ignore) = parse_ranges_and_ids(file);
    merge_ranges(fresh_ranges);
    size_t n_all_fresh_ids = std::ranges::fold_left(fresh_ranges, 0, [](size_t acc, const auto& pair){ return acc + pair.second - pair.first + 1; });
    std::cout << "Number of all fresh ids: " << n_all_fresh_ids << std::endl;
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
