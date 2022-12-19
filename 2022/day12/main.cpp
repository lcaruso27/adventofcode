#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <climits>
#include <deque>
#include <iterator>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 12 2022" << std::endl;
    std::cout << "Hill Climbing - shortest path" << std::endl;
    std::cout << "Usage: day12_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Parsing
void text_to_vertices(
    const std::vector<std::string>& gridlines,
    unsigned char startelem,
    std::vector<std::vector<unsigned int>>& vertices,
    std::vector<unsigned int>& startnodes,
    unsigned int& targetnode
)
{
    unsigned int nrows = gridlines.size();
    unsigned int ncols = gridlines[0].size();

    unsigned int cnode;
    unsigned char celem, cneigh;

    std::vector<unsigned int> cneighbours;
    bool check_top, check_bottom, check_left, check_right;

    for(unsigned int i = 0; i < nrows; i++)
    {
        cnode = i * ncols;
        check_top = i > 0;
        check_bottom = i < (nrows - 1);

        for(unsigned int j = 0; j < ncols; j++)
        {
            cneighbours.clear();
            check_left = j > 0;
            check_right = j < (ncols - 1);
            celem = gridlines[i][j];

            if(celem == 'E')
                targetnode = cnode;

            else
            {
                if(celem == startelem)
                {
                    startnodes.push_back(cnode);
                    celem = 'a'; // make sure still got vertices
                }

                if(check_left) // look left
                {
                    cneigh = gridlines[i][j-1];
                    if(cneigh == 'E') cneigh = 'z';
                    if(celem + 2 > cneigh) cneighbours.push_back(cnode - 1);
                }

                if(check_right) // look right
                {
                    cneigh = gridlines[i][j+1];
                    if(cneigh == 'E') cneigh = 'z';
                    if(celem + 2 > cneigh) cneighbours.push_back(cnode + 1);
                }

                if(check_top) // look top
                {
                    cneigh = gridlines[i-1][j];
                    if(cneigh == 'E') cneigh = 'z';
                    if(celem + 2 > cneigh) cneighbours.push_back(cnode - ncols);
                }

                if(check_bottom) // look bottom
                {
                    cneigh = gridlines[i+1][j];
                    if(cneigh == 'E') cneigh = 'z';
                    if(celem + 2 > cneigh) cneighbours.push_back(cnode + ncols);
                }
            }

            cnode++;
            vertices.push_back(cneighbours);
        }
    }
}

// BFS algo

int steps_shortest_path_search(
    const std::vector<std::vector<unsigned int>>& vertices,
    unsigned int start,
    unsigned int target
    )
{
    unsigned int nvertices = vertices.size();

    std::vector<int> dists(nvertices, INT16_MAX);
    std::vector<bool> explored(nvertices, false);
    std::deque<unsigned int> path = {start};

    std::vector<int> parents(nvertices, INT16_MAX); // showing path for debugging
    parents[start] = start;
    explored[start] = true;
    dists[start] = 0;

    unsigned int cnode;
    int cdist;

    while(!path.empty())
    {
        cnode = path.front();
        path.pop_front();

        for(auto itnode = vertices[cnode].begin(); itnode != vertices[cnode].end(); itnode++)
        {
            if(!explored[*itnode])
            {
                cdist = dists[cnode] + 1;
                parents[*itnode] = cnode;

                if(*itnode == target)
                    return cdist;

                dists[*itnode] = cdist;
                explored[*itnode] = true;
                path.push_back(*itnode);
            }
        }
    }

    return 0;
}

// Shortest path: climbing 0-1
// TODO: factorise neighbour discovery

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line;
    std::vector<std::string> lines;

    while(file >> line)
        lines.push_back(line);

    std::vector<std::vector<unsigned int>> all_vertices;
    unsigned int target_node;
    std::vector<unsigned int> startnodes;
    unsigned char startelem = 'S';

    text_to_vertices(lines, startelem, all_vertices, startnodes, target_node);
    int minsteps = steps_shortest_path_search(all_vertices, startnodes.front(), target_node);

    std::cout << "Fewest steps to reach E from S: " << minsteps << std::endl;
    return 0;
}

// Minimum steps for any 'a' square start

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line;
    std::vector<std::string> lines;

    while(file >> line)
        lines.push_back(line);

    std::vector<std::vector<unsigned int>> all_vertices;
    unsigned int target_node;
    std::vector<unsigned int> startnodes;
    unsigned char startelem = 'a';
    int csteps;
    int minsteps = INT16_MAX;

    text_to_vertices(lines, startelem, all_vertices, startnodes, target_node);

    for(auto snode: startnodes)
    {
        csteps = steps_shortest_path_search(all_vertices, snode, target_node);
        if((csteps < minsteps) && (csteps > 0)) minsteps = csteps;
    }

    std::cout << "Fewest steps to reach E from 'a': " << minsteps << std::endl;

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
