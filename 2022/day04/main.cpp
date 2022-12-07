#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 04 2022" << std::endl;
    std::cout << "Camp cleanup" << std::endl;
    std::cout << "Usage: day04_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Convert line to vector of int
// TODO: possible to factorize

std::vector<int> parse_range(const std::string &line)
{
    std::vector<int> ranges(4);
    size_t pos = 0;
    size_t endpos = 0;

    // Split pair

    std::string pleft, pright;
    std::string pdelim = ",";

    pos = line.find(pdelim);
    endpos = line.size();

    pleft = line.substr(0, pos);
    pright = line.substr(pos + 1, endpos);

    // Split range

    std::string rleft, rright;
    std::string rdelim = "-";

    pos = pleft.find(rdelim);
    endpos = pleft.size();

    ranges[0] = std::stoi(pleft.substr(0, pos));
    ranges[1] = std::stoi(pleft.substr(pos + 1, endpos));

    pos = pright.find(rdelim);
    endpos = pright.size();

    ranges[2] = std::stoi(pright.substr(0, pos));
    ranges[3] = std::stoi(pright.substr(pos + 1, endpos));

    return ranges;
}

// Count pairs with one range fully contained in the other

int main_puzzle1(int argc, char** argv)
{   
    std::ifstream file(argv[1]);
    std::string line;
    std::vector<int> ranges; // start1, end1, start2, end2
    int csum = 0;

    while(file >> line)
    {
        ranges = parse_range(line);

        if(ranges[0] < ranges[2]) // right could be contained in left
                csum += (ranges[3] <= ranges[1]) ? 1 : 0;

        else if(ranges[2] < ranges[0])// left could be contained in right
                csum += (ranges[1] <= ranges[3]) ? 1 : 0;
        
        else if(ranges[0] == ranges[2])
            csum += 1; // always the case
    }

    std::cout << "Sum of pairs where one fully contained the other: " << csum << std::endl;
    return 0;
}

// Count overlapping pairs

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line;
    std::vector<int> ranges; // start1, end1, start2, end2
    int csum = 0;

    while(file >> line)
    {
        ranges = parse_range(line);

        if (ranges[0] < ranges[2])
            csum += (ranges[2] <= ranges[1]) ? 1 : 0;
        else if (ranges[2] < ranges[0])
            csum += (ranges[0] <= ranges[3]) ? 1 : 0;
        else if (ranges[0] == ranges[2])
            csum += 1;
    }

    std::cout << "Sum of overlapping pairs: " << csum << std::endl;
    return 0;
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
