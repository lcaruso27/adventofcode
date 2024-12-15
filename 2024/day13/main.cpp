#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 13 2024" << std::endl;
    std::cout << "Claw Contraption" << std::endl;
    std::cout << "Usage: day13_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

struct Button{
    ulong m_x;
    ulong m_y;
};

using Prize = Button; // alias to distinct both but same information

struct ClawMachine{
    Button m_button_a;
    Button m_button_b;
    Prize m_prize;
};

/// Parse coordinates in line
void parse_coordinates(
    const std::string& i_line,
    ulong& o_x_value,
    ulong& o_y_value
)
{
    std::stringstream line_stream{ i_line };
    std::string token{};
    size_t n_prefix = 2;
    size_t n_suffix = 1;
    while(std::getline(line_stream, token, ' ')){
        if (token.empty())
            continue;
        if (token[0] == 'X')
            o_x_value = std::stoul(token.substr(n_prefix, token.size() - n_prefix - n_suffix));
        else if (token[0] == 'Y')
            o_y_value = std::stoul(token.substr(n_prefix, token.size()));
    }
}

/// Parse claw machines
std::vector<ClawMachine> parse_claw_machines(std::ifstream& io_file)
{
    std::vector<ClawMachine> machines;
    std::string line;
    while(std::getline(io_file, line)){
        ClawMachine machine;
        if (line.empty())
            continue;

        ulong x, y;
        parse_coordinates(line, x, y);

        if (line.find("Button A") != std::string::npos)
            machine.m_button_a = Button{ x, y };
        else if (line.find("Button B") != std::string::npos)
            machine.m_button_b = Button{ x , y };
        else if (line.find("Prize") != std::string::npos){
            machine.m_prize = Prize{ x, y };
            machines.push_back(machine);
        }
    }
    return machines;
}

/// Number of push is valid
bool is_n_push_valid(double i_n_push, long i_max_push)
{
    return i_n_push >= 0
        && (i_max_push == 0 || i_n_push <= i_max_push)
        && i_n_push == std::floor(i_n_push)
        ;
}

/// Token to win prize: if not possible returns 0
ulong min_tokens_to_win_prize(
    const ClawMachine& i_machine,
    ulong i_max_push,
    ulong i_cost_a,
    ulong i_cost_b
    )
{
    auto e11 = static_cast<double>(i_machine.m_button_a.m_x);
    auto e12 = static_cast<double>(i_machine.m_button_b.m_x);
    auto e21 = static_cast<double>(i_machine.m_button_a.m_y);
    auto e22 = static_cast<double>(i_machine.m_button_b.m_y);

    double divisor = (e11 * e22) - (e12 * e21);
    if (std::abs(divisor) <= 1e-5)
        return 0l;

    double n_push_a = (e22 * i_machine.m_prize.m_x - e12 * i_machine.m_prize.m_y) / divisor;
    double n_push_b = (-e21 * i_machine.m_prize.m_x + e11 * i_machine.m_prize.m_y) / divisor;

    ulong min_tokens = (is_n_push_valid(n_push_a, i_max_push) && is_n_push_valid(n_push_b, i_max_push))
        ? static_cast<ulong>(i_cost_a * n_push_a + i_cost_b * n_push_b)
        : 0ul;

    return min_tokens;
}

/// Sum min tokens to win
ulong sum_min_tokens_to_win(
    const std::vector<ClawMachine>& i_machines,
    long i_max_n_push = 100,
    long i_cost_a = 3,
    long i_cost_b = 1
){
    ulong sum_tokens = 0;
    for (const auto& machine : i_machines)
        sum_tokens += min_tokens_to_win_prize(machine, i_max_n_push, i_cost_a, i_cost_b);

    return sum_tokens;
}

/// Fewest tokens to win
int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<ClawMachine> machines = parse_claw_machines(file);
    ulong sum_tokens = sum_min_tokens_to_win(machines);
    std::cout << "Minimum #tokens to win all possible prizes: " << sum_tokens << std::endl;
    return 0;
}

/// Fewest tokens to win with fixed prize distance
int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<ClawMachine> machines = parse_claw_machines(file);
    ulong extra = 1e13;
    std::for_each(
        machines.begin(),
        machines.end(),
        [&extra](auto& machine) {
            machine.m_prize.m_x += extra;
            machine.m_prize.m_y += extra;
        });
    ulong sum_tokens = sum_min_tokens_to_win(machines, 0);
    std::cout << "Minimum #tokens to win all possible prizes wtih fixed prize: " << sum_tokens << std::endl;
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
