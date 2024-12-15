#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <cmath>


std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 06 2024" << std::endl;
    std::cout << "Guard Gallivant" << std::endl;
    std::cout << "Usage: day06_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

struct GridPos{
    size_t m_i;
    size_t m_j;
    char m_direction;

    bool operator<(const GridPos& i_pos)
    {
        if (m_i > i_pos.m_i) return false;
        if (m_i < i_pos.m_i) return true;
        if (m_j > i_pos.m_j) return false;
        if (m_j < i_pos.m_j) return true;
        return false;
    }

    bool operator==(const GridPos& i_pos)
    {
        return (m_i == i_pos.m_i
            && m_j == i_pos.m_j
            && m_direction == i_pos.m_direction);
    }
};

/// Parse input grid
void parse_grid(
    std::ifstream& io_file,
    std::vector<std::string>& o_grid,
    GridPos& o_start_pos)
{
    std::string line;
    o_start_pos = GridPos{ std::string::npos /*m_i*/, std::string::npos /*m_j*/, '^' /*m_direction*/ };
    while (std::getline(io_file, line)){
        o_grid.push_back(line);
        if (o_start_pos.m_j == std::string::npos){
            o_start_pos.m_j = line.find(o_start_pos.m_direction);
            o_start_pos.m_i++; // restart to 0 after std::string::npos
        }
    }

    if (o_start_pos.m_j == std::string::npos)
        throw std::runtime_error("Failed to find start position");
}

/// Is guard stuck in loop (true) or do they leave the map (false)
bool is_guard_stuck(
    const std::vector<std::string>& i_grid,
    GridPos& io_start_pos,
    std::vector<GridPos>& o_visited,
    char i_obstacle = '#')
{
    std::map<char, char> turn_rights{
        { '^', '>' },
        { '>', 'v' },
        { 'v', '<' },
        { '<', '^' }
    };

    std::map<char, std::pair<long, long>> deltas{
        { '^', { -1, 0 } },
        { '>', { 0, 1 } },
        { 'v', { 1, 0 } },
        { '<', { 0, -1 } }
    };

    std::map<char, long> direction_indices{
        { '^', 1 }, // 0001
        { '>', 2 }, // 0010
        { 'v', 4 }, // 0100
        { '<', 8 }  // 1000
    };

    o_visited.clear();

    bool is_stuck = false;
    if (i_grid.empty())
        return is_stuck;

    const size_t n_rows = i_grid.size();
    const size_t n_cols = i_grid.front().size();

    size_t n_indices = n_rows * n_cols;
    // using indices allows quick checks
    std::vector<long> indices(n_indices, 0);

    long index = io_start_pos.m_j + n_cols * io_start_pos.m_i;
    while (io_start_pos.m_i < n_rows && io_start_pos.m_j < n_cols && !is_stuck){
        char pos = i_grid.at(io_start_pos.m_i).at(io_start_pos.m_j);
        if (pos == i_obstacle){
            // need to step back from previous loop
            auto delta = deltas[io_start_pos.m_direction];
            io_start_pos.m_i -= delta.first;
            index -= n_cols * delta.first;
            io_start_pos.m_j -= delta.second;
            index -= delta.second;
            io_start_pos.m_direction = turn_rights[io_start_pos.m_direction];
            continue;
        }

        if (indices[index] == 0)
            o_visited.push_back(io_start_pos);

        long index_direction = direction_indices[io_start_pos.m_direction];
        is_stuck = (indices[index] & index_direction) != 0; // same direction at same index
        indices[index] += index_direction;

        auto delta = deltas[io_start_pos.m_direction];
        io_start_pos.m_i += delta.first;
        index += n_cols * delta.first;
        io_start_pos.m_j += delta.second;
        index += delta.second;
    }

    return is_stuck;
}

/// Count areas visited by guard
long count_visited_positions(
    const std::vector<std::string>& i_grid,
    GridPos& io_start_pos,
    char i_obstacle = '#')
{
    std::vector<GridPos> visited;
    is_guard_stuck(i_grid, io_start_pos, visited, i_obstacle);
    long n_visited = visited.size();
    return n_visited;
}

/// Number of positions visited by the guard
int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::string> grid;
    GridPos start_pos;
    parse_grid(file, grid, start_pos);
    long n_visited = count_visited_positions(grid, start_pos);
    std::cout << "#Visited distinct positions: " << n_visited << std::endl;
    return 0;
}

/// Check positions to place obstacles to create loops: brute-forcing
long count_positions_for_stucking_gard(
    const std::vector<std::string>& i_grid,
    const GridPos& i_start_pos,
    const std::vector<GridPos>& i_candidates,
    char i_obstacle = '#')
{
    long count_stucking_candidates = 0;
    GridPos copy_start_pos(i_start_pos);

    for (const auto& cdte : i_candidates){
        std::vector<std::string> copy_grid(i_grid.begin(), i_grid.end());
        std::vector<GridPos> visited;
        copy_grid.at(cdte.m_i).at(cdte.m_j) = i_obstacle;
        if (is_guard_stuck(copy_grid, copy_start_pos, visited, i_obstacle))
            count_stucking_candidates++;

        copy_start_pos = cdte;
    }

    return count_stucking_candidates;
}

/// Count positions for loops
int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::string> grid;
    GridPos start_pos;
    parse_grid(file, grid, start_pos);
    std::vector<GridPos> candidates;
    {
        GridPos copy_start_pos = start_pos;
        is_guard_stuck(grid, copy_start_pos, candidates);
        candidates.erase(candidates.begin()); // remove start position
    }
    long n_positions_for_stucking = count_positions_for_stucking_gard(grid, start_pos, candidates);
    std::cout << "#Distinct positions for stucking guards: " << n_positions_for_stucking << std::endl;
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
