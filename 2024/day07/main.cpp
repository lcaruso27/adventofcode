#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <sstream>
#include <utility>
#include <algorithm>
#include <array>
#include <functional>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 07 2024" << std::endl;
    std::cout << "Bridge Repair" << std::endl;
    std::cout << "Usage: day07_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

using Equation = std::pair<long, std::vector<long>>;

using BinaryFunc = std::function<long(long, long)>;

/// Parse inputs into sets and test values
std::vector<Equation> parse_equations(std::ifstream& io_file)
{
    std::vector<Equation> all_equations;
    std::string line;
    std::string test_value_delim = ": ";
    while (std::getline(io_file, line)){
        size_t pos = line.find(test_value_delim);
        long result = std::stol(line.substr(0, pos));

        std::stringstream line_stream{ line.substr(pos + test_value_delim.size() )};
        std::string str_number;
        std::vector<long> numbers;
        while (std::getline(line_stream, str_number, ' '))
            numbers.push_back(std::stol(str_number));

        all_equations.push_back(std::make_pair(result, numbers));
    }

    return all_equations;
}

/// Check if equation is true with brute-forcing all * and + operations.
///  Using recursive approach.
template <class Iterator, size_t N>
bool is_equation_rec(
    long i_test_value,
    Iterator first,
    Iterator last,
    long i_accumulator,
    const std::array<BinaryFunc, N>& i_operators
)
{
    if (first == last)
        return i_test_value == i_accumulator;

    if (i_accumulator > i_test_value)
        return false;

    long next_number = (*first++);
    return std::any_of(
        i_operators.begin(),
        i_operators.end(),
        [&](const auto& op){
            return is_equation_rec(i_test_value, first, last, op(i_accumulator, next_number), i_operators);
        }
    );
}

/// Sum of calibration results
/// TODO From redditers: backwards approach is faster.
template <size_t N>
long total_calibrations(
    const std::vector<Equation>& i_equations,
    const std::array<BinaryFunc, N>& i_ops)
{
    long total = 0;
    std::for_each(
        i_equations.begin(),
        i_equations.end(),
        [&total, &i_ops](const Equation& elem){
            if (is_equation_rec(
                elem.first,
                std::next(elem.second.begin()),
                elem.second.end(),
                elem.second.front(),
                i_ops
                ))
                total += elem.first;
        }
    );

    return total;
}

/// Sum of calibration results: two operators
int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<Equation> all_equations = parse_equations(file);
    const std::array<BinaryFunc, 2> ops{ std::plus<long>(), std::multiplies<long>() };
    long total = total_calibrations(all_equations, ops);
    std::cout << "Total of calibration results (two operators): " << total << std::endl;
    return 0;
}

/// Concat operator
struct Concat : BinaryFunc{
    long operator()(long lhs, long rhs){
        return std::stol(std::to_string(lhs) + std::to_string(rhs));
    }
};

/// Sum of calibration results: three operators
int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<Equation> all_equations = parse_equations(file);
    const std::array<BinaryFunc, 3> ops{ std::plus<long>(), std::multiplies<long>(), Concat() };
    long total = total_calibrations(all_equations, ops);
    std::cout << "Total of calibration results (three operators): " << total << std::endl;
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
