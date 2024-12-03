#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <algorithm>
#include <cctype>
#include <numeric>
#include <regex>
#include <utility>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 03 2024" << std::endl;
    std::cout << "Mull It Over" << std::endl;
    std::cout << "Usage: day03_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Sum of mul instructions results

/// Parse input into one instruction
std::string parse_instructions(std::ifstream& i_file)
{
    std::string instructions{};
    std::string line;
    while(std::getline(i_file, line))
        instructions += line;

    return instructions;
}

/// Check if string is a number
bool is_number(const std::string& i_word)
{
    return std::all_of(i_word.begin(), i_word.end(), [](char c) { return std::isdigit(c); });
}

/// Compute correct multiplied values
long sum_multiplied_values(const std::string& i_instructions)
{
    long sum_values = 0;
    size_t prev_start = 0, mul_start = 0, mul_end = 0;
    while (mul_start != std::string::npos && mul_end != std::string::npos){
        mul_start = i_instructions.find("mul(", mul_end);
        if (mul_start == std::string::npos)
            break;
        mul_start += 4; // "mul(" characters
        mul_end = i_instructions.find(")", mul_start);

        size_t mul_sep = i_instructions.find(",", mul_start);
        if (mul_sep > mul_end){
            mul_end = mul_start;
            continue;
        }

        std::string left_part = i_instructions.substr(mul_start, mul_sep - mul_start);
        std::string right_part = i_instructions.substr(mul_sep + 1, mul_end - mul_sep - 1);
        if (!is_number(left_part) || !is_number(right_part)){ // even spaces are considered as invalid number
            mul_end = mul_start;
            continue;
        }
        long left_value = std::stol(left_part);
        long right_value = std::stol(right_part);
            sum_values += left_value * right_value;
    }
    return sum_values;
}

/// Compute correct multipled values with regex
long sum_multiplied_values_with_regex(const std::string& i_instructions)
{
    std::regex word("mul\\((\\d+),(\\d+)\\)");
    auto words_begin = std::sregex_token_iterator(
        i_instructions.begin(),
        i_instructions.end(),
        word,
        {1 /*1st group*/, 2 /*2nd group*/});
    std::sregex_token_iterator words_end;
    long sum_values = 0;
    while (words_begin != words_end){
        long left_value = std::stol(words_begin->str());
        words_begin++;
        long right_value = std::stol(words_begin->str());
        sum_values += left_value * right_value;
        words_begin++;
    }
    return sum_values;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string instructions = parse_instructions(file);
    long sum_values = sum_multiplied_values_with_regex(instructions);

    std::cout << "Sum of multiplications: " << sum_values << std::endl;
    return 0;
}

// Check for do and don't before finding multiplications

/// Sum enabled multiplied values
long sum_enabled_multiplied_values_with_regex(const std::string& i_instructions)
{
    std::regex word("mul\\((\\d+),(\\d+)\\)|do\\(\\)|don't\\(\\)");
    auto words_begin = std::sregex_token_iterator(
        i_instructions.begin(),
        i_instructions.end(),
        word,
        {0 /*match*/, 1 /*1st group*/, 2 /*2nd group*/});
    std::sregex_token_iterator words_end;
    long sum_values = 0;
    bool is_enabled = true;
    while (words_begin != words_end){
        std::string op = words_begin->str();
        if (op == "do()")
            is_enabled = true;
        else if (op == "don't()")
            is_enabled = false;
        else if (op.substr(0, 4) == "mul(" && is_enabled)
        {
            ++words_begin;
            long left_value = std::stol(words_begin->str());
            words_begin++;
            long right_value = std::stol(words_begin->str());
            sum_values += left_value * right_value;
        }
        ++words_begin;
    }

    return sum_values;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string instructions = parse_instructions(file);
    long sum_values = sum_enabled_multiplied_values_with_regex(instructions);
    std::cout << "Sum of enabled multiplications: " << sum_values << std::endl;
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
