#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <algorithm>
#include <cctype>
#include <map>
#include <iterator>
#include <vector>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 01 2023" << std::endl;
    std::cout << "Calibration values reading" << std::endl;
    std::cout << "Usage: day01_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

std::string keep_two_digits(std::string str /*local copy preferred*/)
{
    auto it = std::remove_if(
        str.begin(),
        str.end(),
        [](const unsigned char& cc){return !std::isdigit(cc);}
    );
    str.erase(it, str.end());

    std::string two_digits = !str.empty() ? std::string{str.front(), str.back()} : "0";
    return two_digits;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::string line = "";
    int sum_values = 0 ;

    while(file >> line)
    {
        std::string two_digits = keep_two_digits(line);
        sum_values += std::stoi(two_digits);
    }

    std::cout << "Sum of calibration values: " << sum_values << std::endl;
    return 0;
}

template<class StrIterator>
std::string find_first_digit(
    StrIterator first,
    const StrIterator& last,
    const std::vector<std::pair<std::string, std::string>>& spelled_digit_dict
)
{
    for(; first != last; ++first)
    {
        if(std::isdigit(*first))
            return std::string{*first};
        else
        {
            for(const auto& spelled : spelled_digit_dict)
            {
                std::string candidate = std::string(first, std::next(first, spelled.first.length()));
                if(candidate == spelled.first)
                    return spelled.second;
            }
        }
    }

    return "0";
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::string line = "";
    int sum_values = 0;

    std::vector<std::pair<std::string, std::string>> from_to_digits = {
        {"zero", "0"},
        {"one", "1"},
        {"two", "2"},
        {"three", "3"},
        {"four", "4"},
        {"five", "5"},
        {"six", "6"},
        {"seven", "7"},
        {"eight", "8"},
        {"nine", "9"},
    };

    std::vector<std::pair<std::string, std::string>> reverse_from_to_digits{};
    std::transform(
        from_to_digits.begin(),
        from_to_digits.end(),
        std::back_inserter(reverse_from_to_digits),
        [](const auto& pair){
            std::string rev = std::string(pair.first.rbegin(), pair.first.rend());
            return std::make_pair(rev, pair.second);
        }
    );

    while(file >> line)
    {
        std::string two_digits =
            find_first_digit(line.begin(), line.end(), from_to_digits) +
            find_first_digit(line.rbegin(), line.rend(), reverse_from_to_digits);

        sum_values += std::stoi(two_digits);
    }

    std::cout << "Sum of calibration values: " << sum_values << std::endl;
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
        retcode = main_puzzle2(argc, argv);
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
