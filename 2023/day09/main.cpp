#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <sstream>
#include <deque>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <exception>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 09 2023" << std::endl;
    std::cout << "Mirage maintenance" << std::endl;
    std::cout << "Usage: day09_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

/**
 * Tokenize input stream containing integers delimited with space.
 *
 * @param stream_line istringstream describing line with integers delimited with one space.
 *
 * @return deque of int describing tokenized integers from line.
 */
std::deque<int> tokenize(std::istringstream& stream_line){
    std::deque<int> tokens{};
    int value{};
    while(stream_line >> value)
        tokens.push_back(value);

    return tokens;
}

/**
 * Recursive function for predicted value.
 *
 * @tparam Iterator container iterator type.
 * @tparam T container value type.
 *
 * @param first Iterator describing start of container.
 * @param last Iterator describing end of container.
 * @param target_value T value to reach in all of container to stop recursive calls.
 *
 * @return T value of extrapolated future value.
 */
template<class Iterator, class T> 
T extrapolate_future_value(
    Iterator first,
    Iterator last,
    const T& target_value
){
    if(first == last)
        throw std::runtime_error("No element to check.");

    if(std::all_of(first, last, [&target_value](const T& val){return val == target_value;}))
        return *std::prev(last);

    int inc = *std::prev(last); // store last value to add it later

    std::adjacent_difference(first, last, first); // inplace

    return inc + extrapolate_future_value(std::next(first), last, target_value); // skip first element
}

// First puzzle: extrapolate future value
int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::string line{};
    int sum_predicted_values = 0;
    int target_diff = 0;

    while(std::getline(file, line)){
        std::istringstream stream_line{line};
        std::deque<int> values = tokenize(stream_line);
        sum_predicted_values += extrapolate_future_value(values.begin(), values.end(), target_diff);
    }

    std::cout << "Sum of predicted future values: " << sum_predicted_values << std::endl;
    return 0;
}

// Second puzzle: extrapolate previous value -> reverse from first one
int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::string line{};
    int sum_predicted_values = 0;
    int target_diff = 0;

    while(std::getline(file, line)){
        std::istringstream stream_line{line};
        std::deque<int> values = tokenize(stream_line);
        sum_predicted_values += extrapolate_future_value(values.rbegin(), values.rend(), target_diff);
    }

    std::cout << "Sum of predicted values: " << sum_predicted_values << std::endl;
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
