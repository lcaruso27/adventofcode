#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <iterator>
#include <ranges>
#include <algorithm>
#include <sstream>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 06 2025" << std::endl;
    std::cout << "Trash Compactor" << std::endl;
    std::cout << "Usage: day06_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Parse lines of operations

using NumbersRows = std::vector<std::vector<size_t>>;
using OpsRows = std::vector<char>;
std::pair<NumbersRows, OpsRows> parse_ops_per_line(std::ifstream& i_file)
{
    std::vector<std::string> lines;
    {
        std::string line;
        while (std::getline(i_file, line)) lines.push_back(line);
    }

    // parse number per lines
    NumbersRows numbers_per_lines = lines
        | std::views::take(lines.size() - 1)
        | std::views::transform([](const auto& line)
        {
            std::istringstream iss{ line };
            return std::vector<size_t>{
                std::istream_iterator<size_t>{ iss },
                std::istream_iterator<size_t>()
            };
        })
        | std::ranges::to<std::vector>();

    OpsRows operations = lines.back()
        | std::views::filter([](char c) { return !std::isspace(static_cast<unsigned char>(c)); })
        | std::ranges::to<std::vector>();

    return std::make_pair(numbers_per_lines, operations);
}

using MathProblem = std::pair<std::vector<size_t>, char>;

std::vector<MathProblem> transpose_to_columns(
    const NumbersRows& i_numbers_rows,
    const OpsRows& i_ops_rows
)
{
    if (i_numbers_rows.empty() && i_ops_rows.empty())
        return std::vector<MathProblem>{};

    size_t n_problems = i_numbers_rows.front().size();
    bool is_consistent_numbers = std::all_of(
        std::next(i_numbers_rows.begin()),
        i_numbers_rows.end(),
        [&n_problems](const auto& numbers) { return numbers.size() == n_problems; }
    );
    bool is_consistent_operations = i_ops_rows.size() == n_problems;
    if (!(is_consistent_numbers && is_consistent_operations)) throw std::runtime_error("not transposable");

    // transpose: number per problem
    std::vector<std::vector<size_t>> numbers_per_problem(
        n_problems,
        std::vector<size_t>(i_numbers_rows.size())
    );
    for (auto i : std::views::iota(static_cast<size_t>(0), i_numbers_rows.size()))
    {
        for (auto j : std::views::iota(static_cast<size_t>(0), i_numbers_rows[i].size()))
        {
            numbers_per_problem[j][i] = i_numbers_rows[i][j];
        }
    }

    return std::views::zip(numbers_per_problem, i_ops_rows)
        | std::ranges::to<std::vector<MathProblem>>();
}

size_t total(const MathProblem& i_problem)
{
    if (i_problem.second == '+') return std::ranges::fold_left(i_problem.first, 0, std::plus<size_t>());
    else if (i_problem.second == '*') return std::ranges::fold_left(i_problem.first, 1, std::multiplies<size_t>());
    else
    {
        throw std::runtime_error(std::string("invalid operation ") + std::to_string(i_problem.second));
    }
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    auto [numbers_per_rows, operations_per_rows] = parse_ops_per_line(file);
    std::vector<MathProblem> problems = transpose_to_columns(numbers_per_rows, operations_per_rows);
    size_t grand_total = std::ranges::fold_left(problems, 0, [](size_t acc, const auto& problem){ return acc + total(problem); });
    std::cout << "Grand total of problems: " << grand_total << std::endl;
    return 0;
}

// Read with right-to-left msb

std::vector<MathProblem> parse_problem_right_msb(std::ifstream& i_file)
{
    // parse lines
    std::vector<std::string> lines;
    {
        std::string line;
        while (std::getline(i_file, line)) lines.push_back(line);
    }
    if (lines.empty()) return std::vector<MathProblem>{};

    // transpose and read from right
    std::vector<std::string> numbers_per_column(lines.front().size(), std::string("", lines.size() - 1));
    for (auto i : std::views::iota(static_cast<size_t>(0), lines.size() - 1))
    {
        for (auto j : std::views::iota(static_cast<size_t>(0), lines[i].size()))
        {
            numbers_per_column[j][i] = lines[i][j];
        }
    }

    NumbersRows numbers_right_msb;
    auto it_number = numbers_per_column.rbegin();
    while(it_number != numbers_per_column.rend())
    {
        auto it_pos = std::find_if(
            it_number,
            numbers_per_column.rend(),
            [](const auto& str){ return str.find_first_not_of(' ') == std::string::npos; });

        std::vector<size_t> numbers;
        std::transform(
            it_number,
            it_pos,
            std::back_inserter(numbers), [](const auto& str){ return std::stoul(str);});
        numbers_right_msb.push_back(numbers);

        it_number = it_pos != numbers_per_column.rend() ? it_pos + 1 : it_pos;
    }

    OpsRows operations = lines.back()
        | std::views::filter([](char c) { return !std::isspace(static_cast<unsigned char>(c)); })
        | std::views::reverse
        | std::ranges::to<std::vector>();

    return std::views::zip(numbers_right_msb, operations)
        | std::ranges::to<std::vector<MathProblem>>();
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<MathProblem> problems = parse_problem_right_msb(file);
    size_t grand_total = std::ranges::fold_left(problems, 0, [](size_t acc, const auto& problem){ return acc + total(problem); });
    std::cout << "Grand total of problems: " << grand_total << std::endl;
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
