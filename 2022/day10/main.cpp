#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <numeric>
#include <sstream>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 10 2022" << std::endl;
    std::cout << "Cathode-Ray Tube" << std::endl;
    std::cout << "Usage: day10_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Sum of [20:220:40]th cycle signal strengths

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::string line;
    std::vector<int> values;
    std::string addinst = "addx";
    size_t instlen = addinst.size();

    // parsing
    while(std::getline(file, line))
    {
        values.push_back(0);
        if(line.substr(0, instlen) == addinst)
            values.push_back(std::stoi(line.substr(instlen + 1))); // 2 cycles to add value
    }

    int startcycles = 20;
    int endcycles = 220;
    int stepcycles = 40;

    int regval = 1;
    int sumsignal = 0;
    auto itbegin = values.begin();
    auto citstart = values.begin();
    auto itend = std::next(itbegin, endcycles + 1);
    int ncycles;

    for(auto it = std::next(itbegin, startcycles - 1); it < itend; it += stepcycles)
    {
        regval += std::accumulate(citstart, it, 0);
        ncycles = std::distance(itbegin, it) + 1;
        sumsignal += (ncycles * regval);

        citstart = it;
    }

    std::cout << "Sum of signal strenghts [" ;
    std::cout << startcycles << ":" << endcycles << ":" << stepcycles << "]: ";
    std::cout << sumsignal << std::endl;

    return 0;
}

// Drawing letters

unsigned char drawing_crt(const int& crt_pos, const int& middle, const int& wide)
{
    bool in_sprite = (crt_pos >= middle - wide) && (crt_pos <= middle + wide);
    return in_sprite ? 'X' : ' ';
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::string addinst = "addx";
    size_t instlen = addinst.size();
    std::string line;

    std::vector<int> values;

    // parsing
    while(std::getline(file, line))
    {
        values.push_back(0);
        if(line.substr(0, instlen) == addinst)
            values.push_back(std::stoi(line.substr(instlen + 1))); // 2 cycles to add value
    }

    int wide = 40;
    int sprite_offset = 3 / 2;
    int sprite_middle = 1; // reg init
    int crt_pos = 0;

    std::string crt_row;
    unsigned char crt_draw;
    int ncycles = 1;
    int countpos = 1;

    for(auto it = values.begin(); it != values.end(); it++)
    {
        // Update if new line
        if(countpos > wide) // new line
        {
            std::cout << crt_row << std::endl;
            crt_row.clear();
            crt_pos = 0;
            countpos = 1;
        }

        // Draw
        crt_draw = drawing_crt(crt_pos, sprite_middle, sprite_offset);
        crt_row.push_back(crt_draw);

        // Get register position
        sprite_middle += *it;

        // Update counters
        crt_pos++;
        ncycles++;
        countpos++;
    }

    std::cout << crt_row << std::endl;

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
