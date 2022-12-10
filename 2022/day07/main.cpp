#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <deque>
#include <vector>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 07 2022" << std::endl;
    std::cout << "Device space" << std::endl;
    std::cout << "Usage: day07_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// TODO: Factor stuff in recursive function

// Find sum of folders space with size < 100000 size

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    unsigned int limsize = 100000u;
    unsigned int sumcount = 0;

    std::string line, cfolder;
    std::string line_cd = "$ cd";
    std::deque<std::string> stackpath = {"/"};

    std::deque<unsigned int> stacksize = {0};
    unsigned int tmpsum = 0;
    size_t pos;
    std::string fsize;

    // Pass 1st line (root)
    std::getline(file, line);

    while(std::getline(file, line))
    {
        if(line.substr(0, 4) == line_cd)
        {
            cfolder = line.substr(5) + "/";

            if(cfolder == "../")
            {
                stackpath.pop_back();

                if(tmpsum <= limsize) sumcount += tmpsum;

                // add child folder size
                tmpsum += stacksize.back();
                stacksize.pop_back();
            }
            else
            {
                stackpath.push_back(cfolder);
                stacksize.push_back(tmpsum);
                tmpsum = 0;
            }
        }
        else if(line[0] != '$')
        {
            pos = line.find(" ");
            fsize = line.substr(0, pos);
            if(fsize != "dir") // this is a file
            {
                tmpsum += std::stoi(fsize);
            }
        }
        // else // ls command do nothing
    }

    // Update last folder

    while(!stacksize.empty())
    {
        if(tmpsum <= limsize) sumcount += tmpsum;
        tmpsum += stacksize.back();
        stacksize.pop_back();
    }

    std::cout << "Total size of directories with less than " << limsize << ": ";
    std::cout << sumcount << std::endl;

    return 0;
}

// Find smallest folder

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line, cfolder;
    std::string line_cd = "$ cd";
    std::deque<std::string> stackpath = {"/"};

    std::deque<unsigned int> stacksize = {0};
    std::deque<unsigned int> allsize;
    unsigned int tmpsum = 0;
    unsigned int sumcount;
    size_t pos;
    std::string fsize;

    // Pass 1st line (root)
    std::getline(file, line);

    while(std::getline(file, line))
    {
        if(line.substr(0, 4) == line_cd)
        {
            cfolder = line.substr(5) + "/";

            if(cfolder == "../")
            {
                stackpath.pop_back();
                allsize.push_back(tmpsum);

                // add child folder size
                tmpsum += stacksize.back();
                stacksize.pop_back();
                allsize.push_back(tmpsum);
            }
            else
            {
                stackpath.push_back(cfolder);
                stacksize.push_back(tmpsum);
                if(!allsize.empty()) allsize.pop_back();
                tmpsum = 0;
            }
        }
        else if(line[0] != '$')
        {
            pos = line.find(" ");
            fsize = line.substr(0, pos);
            if(fsize != "dir") // this is a file
            {
                tmpsum += std::stoi(fsize);
            }
        }
        // else // ls command do nothing
    }

    // Unstack for updating folders
    while(!stacksize.empty())
    {
        allsize.push_back(tmpsum);
        tmpsum += stacksize.back();
        stacksize.pop_back();
    }

    // Find smallest required
    unsigned int totalmem = 70000000;
    unsigned int requiredmem = 30000000;
    unsigned int minimem = 0;

    // Sorting
    std::sort(allsize.begin(), allsize.end());

    int targetlim = requiredmem - totalmem + allsize.back();

    if(targetlim <= 0)
        minimem = 0;
    else
    {
        while((!allsize.empty()) && (minimem < targetlim))
        {
            minimem = allsize.front();
            allsize.pop_front();
        }
    }

    std::cout << "Size of smallest folder to delete to free up space: " << minimem << std::endl;
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
