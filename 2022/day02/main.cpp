#include <iostream>
#include <fstream>
#include <string>
#include <exception>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 02 2022" << std::endl;
    std::cout << "Rock papper scissors tournament" << std::endl;
    std::cout << "Usage: day02_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Comment

int main_puzzle1(int argc, char** argv)
{   
    std::ifstream file(argv[1]);
    char opp, mplay;
    int sumpoints = 0;

    // Rock 1, Paper 2, Scissors 3
    // Loss 0, Draw 3, Win 6

    // AX -> 0 / 0 -> 3 -> 1
    // AY -> 0 / 1 -> 6 -> 2
    // AZ -> 0 / 2 -> 0 -> 0

    // BX -> 1 / 0 -> 0 -> 0
    // BY -> 1 / 1 -> 3 -> 1
    // BZ -> 1 / 2 -> 6 -> 2

    // CX -> 2 / 0 -> 6 -> 2
    // CY -> 2 / 1 -> 0 -> 0
    // CZ -> 2 / 2 -> 3 -> 1


    while(file >> opp >> mplay)
    {
        // Not much readability
        sumpoints += 1 + mplay - 'X' + 3 * ((mplay - 'X' + 4 - opp + 'A') % 3);
    }

    std::cout << "Total points with strategy (play): " << sumpoints << std::endl;
    return 0;
}

// Comment

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    char opp, mplay;
    int sumpoints = 0;

    // Rock 1, Paper 2, Scissors 3
    // Loss 0, Draw 3, Win 6

    // AX -> 0 / 0 -> 2
    // AY -> 0 / 3 -> 0
    // AZ -> 0 / 6 -> 1

    // BX -> 1 / 0 -> 0
    // BY -> 1 / 3 -> 1
    // BZ -> 1 / 6 -> 2

    // CX -> 2 / 0 -> 1
    // CY -> 2 / 3 -> 2
    // CZ -> 2 / 6 -> 0


    while(file >> opp >> mplay)
    {
        // Not much readability
        sumpoints += 3 * (mplay - 'X') + 1 + ((mplay - 'X' + 2 + opp - 'A') % 3);
    }

    std::cout << "Total points with strategy (outcome): " << sumpoints << std::endl;
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
