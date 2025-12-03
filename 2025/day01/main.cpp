#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <utility>
#include <algorithm>

// TODO: use L/R to identify sign for rotation
// TODO: use C++20

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 01 2025" << std::endl;
    std::cout << "Secret Entrance" << std::endl;
    std::cout << "Usage: day01_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

using SafeInstruction = std::pair<char, unsigned>;

std::vector<SafeInstruction> parse_sequence(std::ifstream& i_file)
{
    std::vector<std::pair<char, unsigned>> sequence;
    std::string word{};
    while (i_file >> word)
    {
        sequence.emplace_back(word.at(0), std::stoi(word.substr(1)));
    }
    return sequence;
}

void update_position(const SafeInstruction& i_instruction, unsigned i_n_positions, unsigned& io_position)
{
    io_position += i_instruction.first == 'R' ? i_instruction.second % i_n_positions : i_n_positions - (i_instruction.second % i_n_positions);
    io_position %= i_n_positions;
}

bool update_position_overflow(const SafeInstruction& i_instruction, unsigned i_n_positions, unsigned& io_position)
{
    unsigned delta = i_instruction.second % i_n_positions;
    bool is_overflow = io_position != 0;
    is_overflow = is_overflow && (i_instruction.first == 'R' ? io_position + delta >= i_n_positions : io_position <= delta);
    io_position += i_instruction.first == 'R' ? delta : i_n_positions - delta;
    io_position %= i_n_positions;
    return is_overflow;
}

unsigned count_positions(
    const std::vector<SafeInstruction>& i_sequence,
    unsigned i_target_position,
    unsigned i_n_positions,
    unsigned i_initial_position
)
{
    unsigned position = i_initial_position;
    unsigned count = 0;
    std::for_each(
        i_sequence.begin(),
        i_sequence.end(),
        [&i_target_position, &i_n_positions, &position, &count](const auto& instruction){
            update_position(instruction, i_n_positions, position);
            if (position == i_target_position) count++;
        });
    return count;
}

unsigned count_clicks(
    const std::vector<SafeInstruction>& i_sequence,
    unsigned i_target_position,
    unsigned i_n_positions,
    unsigned i_initial_position
)
{
    unsigned position = i_initial_position;
    unsigned count = 0;
    std::for_each(
        i_sequence.begin(),
        i_sequence.end(),
        [&i_target_position, &i_n_positions, &position, &count](const auto& instruction){
            unsigned quotient = instruction.second / i_n_positions;
            count += quotient;
            if (update_position_overflow(instruction, i_n_positions, position)) count++;
        });
    return count;
}

// Comment

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<SafeInstruction> sequence = parse_sequence(file);
    unsigned count = count_positions(sequence, 0 /*target*/, 100 /*n_positions*/, 50 /*initial_position*/);
    std::cout << "password is: " << count << std::endl;
    return 0;
}

// Comment

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<SafeInstruction> sequence = parse_sequence(file);
    unsigned count = count_clicks(sequence, 0 /*target*/, 100 /*n_positions*/, 50 /*initial_position*/);
    std::cout << "password is: " << count << std::endl;
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
