#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <queue>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 06 2022" << std::endl;
    std::cout << "Tuning trouble" << std::endl;
    std::cout << "Usage: day06_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

bool has_duplicates(const std::string& str)
{
    for(auto it = str.begin(); it != str.end(); it++)
    {
        for(auto jit = it + 1; jit != str.end(); jit++)
        {
            if((*it) == (*jit))
                return 1;
        }        
    }

    return 0;
}

// Find start-of-packet marker index

int main_puzzle1(int argc, char** argv)
{   
    std::ifstream file(argv[1]);
    std::string line;

    file >> line;

    unsigned int nlength = 4u;

    std::string token = line.substr(0, nlength);
    size_t ix = 0;
    size_t cprocessed;

    while(has_duplicates(token))
        token = line.substr(++ix, nlength);

    cprocessed = ix + nlength;
    
    std::cout << "[" << nlength << "-code] Characters processed before first marker: ";
    std::cout << cprocessed << std::endl;

    return 0;
}

// Comment

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line;

    file >> line;

    unsigned int nlength = 14u;

    std::string token = line.substr(0, nlength);
    size_t ix = 0;
    size_t cprocessed;

    while(has_duplicates(token))
        token = line.substr(++ix, nlength);

    cprocessed = ix + nlength;
    
    std::cout << "[" << nlength << "-code] Characters processed before first marker: ";
    std::cout << cprocessed << std::endl;
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
