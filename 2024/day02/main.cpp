#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <utility>
#include <iterator>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 02 2024" << std::endl;
    std::cout << "Red-nosed reports" << std::endl;
    std::cout << "Usage: day02_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Count safe reports

/// Parsing report levels
std::vector<std::vector<long>> parse_reports(std::ifstream& i_report)
{
    std::vector<std::vector<long>> reports;
    std::string line{};
    while(std::getline(i_report, line)){
        std::stringstream line_stream{line};
        std::vector<long> report_line;
        long value{};
        while (line_stream >> value)
            report_line.push_back(value);
        reports.push_back(report_line);
    }

    return reports;
}

/// Check if report is safe
bool is_report_safe(
    const std::vector<long>& i_levels,
    long min_exclusive_delta = 0,
    long max_exclusive_delta = 4)
{
    if (i_levels.size() < 2)
        return true;

    bool is_ordered = i_levels.at(0) < i_levels.at(1) /*ascending order*/
        ? std::is_sorted(i_levels.begin(), i_levels.end())
        : std::is_sorted(i_levels.rbegin(), i_levels.rend())
        ;

    if (!is_ordered)
        return false;

    std::vector<long> deltas;
    std::transform(
        i_levels.begin(),
        std::prev(i_levels.end()),
        std::next(i_levels.begin()),
        std::back_inserter(deltas),
        [](const auto& i_left, const auto& i_right) { return std::abs(i_right - i_left);}
    );

    auto [min_delta, max_delta] = std::minmax_element(deltas.begin(), deltas.end());

    return (*min_delta > min_exclusive_delta && *max_delta < max_exclusive_delta);
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::vector<long>> reports = parse_reports(file);
    std::vector<bool> all_status(reports.size());
    std::transform(
        reports.begin(),
        reports.end(),
        all_status.begin(),
        [](const auto& i_levels) { return is_report_safe(i_levels); }
    );
    long n_safe_reports = std::count(all_status.begin(), all_status.end(), true);

    std::cout << "#Safe reports: " << n_safe_reports << std::endl;
    return 0;
}

// Count safe reports with tolerance

template <typename It>
bool is_report_safe_with_dampener(
    It first,
    It last,
    bool is_ascending,
    long min_exclusive_delta = 0,
    long max_exclusive_delta = 4,
    long max_n_errors = 2)
{
    int n_errors = 0;
    It next = first + 1;
    while (n_errors < 2 && next != last){
        long delta = is_ascending ? *next - *first : *first - *next;
        if (!(delta > min_exclusive_delta && delta < max_exclusive_delta))
        {
            n_errors++;
            next++;
            continue;
        }
        first = next++;
    }

    return n_errors < max_n_errors;
}

/// Check if report is safe while tolerating one error
/// Brute-forcing every sublist with one element off
bool is_report_safe_with_dampener(
    const std::vector<long>& i_levels,
    long min_exclusive_delta = 0,
    long max_exclusive_delta = 4,
    long max_n_errors = 2)
{
    bool is_safe = false;
    for (
        auto it = i_levels.begin(),
        next_it = std::next(i_levels.begin());
        it != i_levels.end();
        ++it,
        ++next_it)
    {
        std::vector<long> candidates(i_levels.begin(), it);
        candidates.insert(candidates.end(), next_it, i_levels.end());
        is_safe = is_safe || is_report_safe(candidates, min_exclusive_delta, max_exclusive_delta);
    }
    return is_safe;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::vector<long>> reports = parse_reports(file);
    std::vector<bool> all_status(reports.size());
    std::transform(
        reports.begin(),
        reports.end(),
        all_status.begin(),
        [](const auto& i_levels) { return is_report_safe_with_dampener(i_levels); }
    );
    long n_safe_reports = std::count(all_status.begin(), all_status.end(), true);

    std::cout << "#Safe reports with problem dampener: " << n_safe_reports << std::endl;
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
