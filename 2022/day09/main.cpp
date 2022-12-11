#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <utility>
#include <cmath>
#include <vector>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 09 2022" << std::endl;
    std::cout << "Rope bridge" << std::endl;
    std::cout << "Usage: day09_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// #Positions visited by the tail at least once

// Moving both head and tails

void move_rope(
    const unsigned char& dir,
    std::pair<int, int>& ppos
    )
{
    switch(dir)
    {
        case 'U':
            ppos.first--;
            break;
        case 'D':
            ppos.first++;
            break;
        case 'L':
            ppos.second--;
            break;
        case 'R':
            ppos.second++;
            break;
    }
}

double calc_distance(
    std::pair<int, int> hpos,
    std::pair<int, int> tpos
)
{
    double drow = hpos.first - tpos.first;
    double dcol = hpos.second - tpos.second;

    return std::sqrt((drow * drow) + (dcol * dcol));
}


int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    unsigned char hdir;
    unsigned int nsteps;

    unsigned int poscount = 1;
    double limdist = 2;
    std::pair<int, int> head_pos = {0, 0};
    std::pair<int, int> tail_pos = {0, 0};
    std::pair<int, int> tmp_pos;
    double cdist;

    std::vector<std::pair<int, int>> unique_tailpos = {{0, 0}};
    auto it = unique_tailpos.begin();

    while(file >> hdir >> nsteps)
    {
        for(unsigned int ii = 0; ii < nsteps; ii++)
        {
            // Update knots position
            tmp_pos = head_pos;
            move_rope(hdir, head_pos);

            // Update tail position
            cdist = calc_distance(head_pos, tail_pos);
            if(cdist >= limdist)
            {
                tail_pos = tmp_pos; // previous head position: special case with 2 knots
                it = std::find(unique_tailpos.begin(), unique_tailpos.end(), tail_pos);
                if(it == unique_tailpos.end())
                {
                    unique_tailpos.push_back(tail_pos);
                    poscount++;
                }


            }
        }
    }

    std::cout << "#Positions visited by the tail at least once: " << poscount << std::endl;
    return 0;
}

// 10-knots rope: #Positions visited by the tail at least once

// Update tail position: handle up, down, left, right, and diagonal

void update_tail(
    const std::pair<int, int>& headpos,
    std::pair<int, int>& tailpos
)
{
    double drow = headpos.first - tailpos.first;
    double dcol = headpos.second - tailpos.second;

    if(drow != 0)
        tailpos.first += drow > 0 ? 1 : -1;

    if(dcol != 0)
        tailpos.second += dcol > 0 ? 1 : -1;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    unsigned char hdir;
    unsigned int nsteps;

    unsigned int poscount = 1;
    double limdist = 2;

    std::vector<std::pair<int, int>> rope_knots(10);
    for(auto& knot: rope_knots) knot = std::make_pair(0, 0); // init positions

    std::pair<int, int> chead;
    double cdist;

    std::vector<std::pair<int, int>> unique_tailpos = {{0, 0}};
    auto itpos = unique_tailpos.begin();
    auto lasthead = std::prev(rope_knots.end());

    while(file >> hdir >> nsteps)
    {
        for(unsigned int ii = 0; ii < nsteps; ii++)
        {
            // Update head position
            move_rope(hdir, rope_knots.front());

            // Update tails
            for(auto rit = std::next(rope_knots.begin()); rit != rope_knots.end(); rit++)
            {
                chead = *std::prev(rit);
                // Update tail position
                cdist = calc_distance(chead, *(rit));
                if(cdist >= limdist)
                {
                    update_tail(chead, *rit);
                    if(rit == lasthead) // count tail position
                    {
                        itpos = std::find(unique_tailpos.begin(), unique_tailpos.end(), *rit);
                        if(itpos == unique_tailpos.end())
                        {
                            unique_tailpos.push_back(*rit);
                            poscount++;
                        }
                    }
                }
            }
        }
    }

    std::cout << "[10-knots] #Positions visited by the tail at least once: " << poscount << std::endl;
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
