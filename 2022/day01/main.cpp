#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <numeric>
#include <vector>
#include <algorithm>
#include <functional>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 01 2022" << std::endl;
    std::cout << "About maximum calories carried." << std::endl;
    std::cout << "Usage: day01_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Maximum calories carried among elves

int main_max(int argc, char** argv)
{   
    std::ifstream file(argv[1]);
    int mmax = 0;
    int ix_max = 0;
    int csum = 0;
    int cix = 0;
    std::string line = "";
    bool updatemax;

    while(std::getline(file, line))
    {
        updatemax = (line.empty()) || (file.peek() == EOF);
        csum += line.empty() ? 0 : std::stoi(line);
        
        if(updatemax)
        {
            if (csum > mmax)
            {
                mmax = csum;
                ix_max = cix;
            }

            cix++;
            csum = 0;
        }
    }

    std::cout << "Maximum calories carried by " << ix_max + 1 << "th elf" << std::endl;
    std::cout << "Carrying " << mmax << " calories" << std::endl;

    return 0;
}

// Sum of top 3 max calories carried by elves

// Naive solution
int main_top3max(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    int mmaxs[3] = {0, 0, 0};
    int ixs_max[3] = {0, 0, 0};
    int csum = 0;
    int cix = 0;
    std::string line = "";
    bool updatemax;

    while(std::getline(file, line))
    {
        updatemax = (line.empty()) || (file.peek() == EOF);
        csum += line.empty() ? 0 : std::stoi(line);
        
        if(updatemax)
        { // Possible to vectorize
           
           // Please a stack
            if (csum >= mmaxs[0]) 
            {
                mmaxs[2] = mmaxs[1];
                ixs_max[2] = ixs_max[1];
                
                mmaxs[1] = mmaxs[0];
                ixs_max[1] = ixs_max[0];
                
                mmaxs[0] = csum;
                ixs_max[0] = cix;
            }
            else if(csum >= mmaxs[1])
            {
                mmaxs[2] = mmaxs[1];
                ixs_max[2] = ixs_max[1];

                mmaxs[1] = csum;
                ixs_max[1] = cix;
            }
            else if(csum >= mmaxs[2])
            {
                mmaxs[2] = csum;
                ixs_max[2] = cix;
            }

            cix++;
            csum = 0;
        }
    }

    int sum_mmax = std::accumulate(mmaxs, mmaxs + 3, 0);

    std::cout << "Top 3 maximum calories carried by ";
    std::cout << ixs_max[0] + 1 << "th, ";
    std::cout << ixs_max[1] + 1 << "th, ";
    std::cout << ixs_max[2] + 1 << "th elves" << std::endl;
    std::cout << mmaxs[0] << " | " << mmaxs[1] << " | " << mmaxs[2] << std::endl;
    std::cout << "Total carrying " << sum_mmax << " calories" << std::endl;
    return 0;
}

// Sorting & vector solution

int main_top3max_vector(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<int> mmaxs = {0, 0, 0, 0};
    // vecdequeue -> applies partial sort to minimum

    int csum = 0;
    std::string line = "";
    bool updatemax;

    while(std::getline(file, line))
    {
        updatemax = (line.empty()) || (file.peek() == EOF);
        csum += line.empty() ? 0 : std::stoi(line);
        
        if(updatemax)
        {

            if(csum >= mmaxs[2]) // Worth to apply algo
            {
                mmaxs[3] = csum;
                // Sorting
                std::sort(mmaxs.begin(), mmaxs.end(), std::greater<int>());
            }
            
            csum = 0;
        }
    }

    int sum_mmax = std::accumulate(mmaxs.begin(), mmaxs.begin() + 3, 0);

    std::cout << "Top 3 maximum calories" << std::endl;
    std::cout << mmaxs[0] << " | " << mmaxs[1] << " | " << mmaxs[2] << std::endl;
    std::cout << "Total carrying " << sum_mmax << " calories" << std::endl;
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
        retcode = main_max(argc, argv);

        std::cout << HEADER_DELIM << " 2nd puzzle " << HEADER_DELIM << std::endl;
        retcode += main_top3max(argc, argv);
        retcode += main_top3max_vector(argc, argv);
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
