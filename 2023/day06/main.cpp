#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <sstream>
#include <vector>
#include <cctype>
#include <cmath>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 06 2023" << std::endl;
    std::cout << "Boat racing" << std::endl;
    std::cout << "Usage: day06_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// TODO: improve parsing strategy
std::vector<size_t> parse_values(const std::string& line){
    std::vector<size_t> values{};
    bool parsing_num = false;
    std::string tmp = "";
    for(const char& c : line)
    {
        if(std::isdigit(c))
        {
            parsing_num = true;
            tmp.push_back(c);
        }
        if(!std::isdigit(c) && parsing_num)
        {
            parsing_num = false;
            values.push_back(std::stoi(tmp));
            tmp = "";
        }
    }

    // Get last number
    if(parsing_num)
        values.push_back(std::stoi(tmp));

    return values;
}

// Naive: with loop
// size_t num_to_be_faster_naive(
//     const size_t& time,
//     const size_t& dist_record,
//     const size_t& speed_inc = 1
// ){
//     size_t count_beat = 0;
//     size_t i = 0;
//     while(++i < time){
//         size_t dist = i * speed_inc * (time - i);

//         if(dist > dist_record)
//             count_beat++;
//     }

//     return count_beat;
// }

// Analytical
// y > d
// -speed_inc * i² + speed_inc * time * i - d > 0
template<typename T>
T num_to_be_faster(
    const T& time,
    const T& distance,
    const T& speed_inc = 1
)
{
    T num_ways = 0;
    double delta = static_cast<double>(std::pow(speed_inc * time, 2) - 4 * speed_inc * distance);
    if(delta <= 0)
        return num_ways;

    double sqrt_delta = std::sqrt(delta);
    double divide = - 2.0 * speed_inc;
    double left = std::floor((-1.0 * speed_inc * time + sqrt_delta) / divide + 1);
    double right = std::ceil((-1.0 * speed_inc * time - sqrt_delta) / divide - 1) ;
    num_ways = static_cast<T>(right - left + 1);

    return num_ways;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    size_t nlines = 2;
    std::string line{};
    std::vector<std::vector<size_t>> all_values /*0: time, 1: records*/;
    for(size_t i = 0; i < nlines; i++){
        while(std::getline(file, line))
            all_values.push_back(parse_values(line));
    }

    if(all_values.empty())
        return 1;

    size_t prod_n_ways = 1;
    size_t n_races = all_values[0].size();
    size_t speed_inc = 1; // mm/ms
    for(size_t i = 0; i < n_races; i++){
        size_t count_beat = num_to_be_faster(
            all_values[0][i] /*time*/,
            all_values[1][i] /*record*/,
            speed_inc);

        prod_n_ways *= count_beat;
    }

    std::cout << "Prod number of ways to be faster: " << prod_n_ways << std::endl;
    return 0;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    size_t nlines = 2;
    std::string line{};
    std::vector<unsigned long long> all_values /*0: durations, 1: records*/;
    for(size_t i = 0; i < nlines; i++){
        while(std::getline(file, line)){
            std::string num_str{};
            std::copy_if(
                line.begin(),
                line.end(),
                std::back_inserter(num_str),
                [](const char& cc){return std::isdigit(cc);}
            );
            all_values.push_back(std::stoll(num_str));
        }
    }

    unsigned long long speed_inc = 1;
    unsigned long long count_beat = num_to_be_faster(
        all_values[0],
        all_values[1],
        speed_inc
    );

    std::cout << "Number of ways to be faster: " << count_beat << std::endl;
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
