#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <set>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 03 2022" << std::endl;
    std::cout << "Rucksack organisation" << std::endl;
    std::cout << "Usage: day03_main input_file" << std::endl;
    std::cout << "" << std::endl;

    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;

    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Sum up priorities of unique common letter in compartiments

// Convert char letter to priority number

int priority(const char &c)
{ 
    return c < 'a' ? c - 'A' + 27 : c - 'a' + 1;
}

// Find first common element (O(n2))
// Could use std::set_intersection: need compartments as sets

std::string::iterator intersection(
    std::string::iterator first1, 
    std::string::iterator last1,
    std::string::iterator first2,
    std::string::iterator last2)
{
    auto cit = first2;
    while(first1 != last1)
    {
        while((cit != last2) && (*first1 != *cit))
            ++cit;
        
        if(*first1 == *cit)
            return first1;
        
        else
        {
            ++first1;
            cit = first2;
        }
    }

    return last2;
}

int main_puzzle1(int argc, char** argv)
{   
    std::ifstream file(argv[1]);
    std::string line;
    std::string::iterator last_comp1, common;
    unsigned csum = 0;

    while(file >> line)
    {
        last_comp1 = line.begin() + line.size() / 2;
        common = intersection(line.begin(), last_comp1, last_comp1, line.end());
        
        if(common != line.end())
            csum += priority(*common);
    }

    std::cout << "Sum of priorities from common items: " << csum << std::endl;

    return 0;
}

// Comment

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line1, line2, line3;
    std::set<char> set1, set2, set3, common12, allcommon;
    unsigned csum = 0;

    while(file >> line1 >> line2 >> line3)
    {
        // Read 3 lines and convert to sets
        set1 = std::set<char>(line1.begin(), line1.end());
        set2 = std::set<char>(line2.begin(), line2.end());
        set3 = std::set<char>(line3.begin(), line3.end());

        // Find intersections

        common12.clear();
        std::set_intersection(
            set1.begin(), set1.end(), 
            set2.begin(), set2.end(), 
            std::inserter(common12, common12.begin()));
        

        allcommon.clear();
        std::set_intersection(
            set3.begin(), set3.end(),
            common12.begin(), common12.end(),
            std::inserter(allcommon, allcommon.begin())
        );

        if(allcommon.begin() != allcommon.end())
            csum += priority(*allcommon.begin());
    }

    std::cout << "Sum of priorities for badges: " << csum << std::endl;
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
