#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 08 2022" << std::endl;
    std::cout << "Tree house" << std::endl;
    std::cout << "Usage: day08_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Count trees visible outside the forest

// Iterate through columns

template<typename It2D, typename It, class UnPredicate>
bool all_of_col(It2D first, It2D last, It col, UnPredicate comp)
{
    for(; first != last; first++)
    {
        if(!comp((*first)[col]))
            return false;
    }

    return true;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::vector<unsigned char>> forest;
    std::vector<unsigned char> row;
    std::string line;

    // parsing
    while(file >> line)
    {
        row.clear();
        for(const auto& cc: line)
            row.push_back(cc - '0');

        forest.push_back(row);
    }

    unsigned int nrows = forest.size();
    unsigned int ncols = forest.front().size();

    // Init with edges
    int sumvisible = 2 * ncols + 2 * (nrows - 2);
    unsigned char tmp;
    bool left, top, right, bottom;

    auto check_height = [&tmp](const unsigned char& elem){return tmp > elem;};

    // Check inside
    for(unsigned int ii = 1; ii < nrows - 1; ii++)
    {
        for(unsigned int jj = 1; jj < ncols - 1; jj++)
        {
            tmp = forest[ii][jj];

            left = std::all_of(forest[ii].begin(), std::next(forest[ii].begin(), jj), check_height);
            right = std::all_of(std::next(forest[ii].begin(), jj + 1), forest[ii].end(), check_height);
            bottom = all_of_col(std::next(forest.begin(), ii + 1), forest.end(), jj, check_height);
            top = all_of_col(forest.begin(), std::next(forest.begin(), ii), jj, check_height);

            sumvisible += (left || right || bottom || top) ? 1 : 0;
        }
    }

    std::cout << "#Visible trees from outside the forest: " << sumvisible << std::endl;

    return 0;
}

// Highest scenic score

// Count while if

template<typename It, class UnPredicate, class T>
T count_while_if(It first, It last, T init, UnPredicate comp)
{
    while((first != last) && (comp(*(first++))))
        init++;

    return init;
}

template<typename It2D, typename It, class UnPredicate, class T>
T count_while_if_col(It2D first, It2D last, It col, T init, UnPredicate comp)
{
    while((first != last) && (comp((*first++)[col])))
        init++;

    return init;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::vector<unsigned char>> forest;
    std::vector<unsigned char> row;
    std::string line;
    
    // parsing
    while(file >> line)
    {
        row.clear();
        for(const auto& cc: line)
            row.push_back(cc - '0');

        forest.push_back(row);
    }

    unsigned int nrows = forest.size();
    unsigned int ncols = forest.front().size();

    // Init with edges
    unsigned char tmp;
    int sleft, stop, sright, sbottom, score;
    int maxscore = 0;
    int init = 1;

    auto check_height = [&tmp](const unsigned char& elem){return tmp > elem;};

    for(unsigned int ii = 1; ii < nrows - 1; ii++)
    {
        for (unsigned int jj = 1; jj < ncols - 1; jj++)
        {
            // compute score

            tmp = forest[ii][jj];

            sleft = count_while_if(std::prev(forest[ii].rend(), jj), std::prev(forest[ii].rend(), 1), init, check_height);
            sright = count_while_if(std::next(forest[ii].begin(), jj + 1), std::prev(forest[ii].end(), 1), init, check_height);
            sbottom = count_while_if_col(std::next(forest.begin(), ii + 1), std::prev(forest.end(), 1), jj, init, check_height);
            stop = count_while_if_col(std::prev(forest.rend(), ii), std::prev(forest.rend(), 1), jj, init, check_height);

            score = sleft * sright * stop * sbottom;

            if(score > maxscore) maxscore = score;
        }
    }

    std::cout << "Highest scenic score: " << maxscore << std::endl;
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
