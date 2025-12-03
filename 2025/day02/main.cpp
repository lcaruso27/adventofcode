#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <utility>
#include <vector>
#include <ranges>
#include <iterator>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 02 2025" << std::endl;
    std::cout << "Gift Shop" << std::endl;
    std::cout << "Usage: day02_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Parsing input as pair of integers
std::vector<std::pair<size_t, size_t>> parse_ranges(std::ifstream& i_file)
{
    std::string line;
    std::vector<std::pair<size_t, size_t>> ranges;
    while (std::getline(i_file, line))
    {
        for (const auto& pair_range : line
            | std::views::split(',')
            | std::views::transform([](auto&& str_range) -> std::pair<size_t, size_t> {
                auto numbers = str_range
                    | std::views::split('-')
                    | std::views::transform([](auto&& token){
                        return std::stoul(std::string(token.data(), token.size()));
                });
                auto it = numbers.begin();
                size_t first = *it;
                if (it == numbers.end()) throw std::runtime_error("only 1 number found in range");
                size_t second = *(++it);
                if (++it != numbers.end()) throw std::runtime_error("more than 2 numbers found in range");
                return { first, second };
            }))
        {
            ranges.push_back(pair_range);
        }
    }
    return ranges;
}

bool is_number_twice(const std::string& i_number)
{
    size_t middle = i_number.size() / 2;
    return i_number.substr(0, middle) == i_number.substr(middle);
}

template <typename Op>
size_t sum_invalid_ids(std::vector<std::pair<size_t, size_t>> i_ranges, Op i_operation)
{
    size_t sum = 0;
    for (const auto& range : i_ranges)
    {
        auto range_view = std::views::iota(range.first, range.second + 1)
            | std::views::filter([&i_operation](size_t number){
                return i_operation(std::to_string(number)); });
        sum += std::ranges::fold_left(range_view, size_t{0}, std::plus{});
    }
    return sum;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::pair<size_t, size_t>> ranges = parse_ranges(file);
    std::cout << "Sum of invalid IDs: " << sum_invalid_ids(ranges, is_number_twice) << std::endl;
    return 0;
}

bool is_multiple_sequence(const std::string& i_number)
{
    if (i_number.empty()) return false;
    bool sequence_found = false;
    for (auto width : std::views::iota(1u, i_number.size() / 2 + 1))
    {
        auto windows = i_number
            | std::views::chunk(width)
            | std::views::transform([](const auto& parts){ return std::string(parts.data(), parts.size()); });
        sequence_found = std::ranges::adjacent_find(windows, std::ranges::not_equal_to() ) == windows.end();
        if (sequence_found) break;
    }
    return sequence_found;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::pair<size_t, size_t>> ranges = parse_ranges(file);
    std::cout << "Sum of invalid IDs: " << sum_invalid_ids(ranges, is_multiple_sequence) << std::endl;
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
