#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <array>
#include <utility>
#include <ranges>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 04 2025" << std::endl;
    std::cout << "Printing Department" << std::endl;
    std::cout << "Usage: day04_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Parsing inputs
using Grid = std::vector<std::string>;
using Direction = std::pair<int, int>;
static constexpr std::array<Direction, 8> DELTA_NEIGH = {
        Direction{ -1, -1 } /*top left*/, Direction{ -1, 0 } /*mid top*/, Direction{ -1, 1 } /*top right*/,
        Direction{ 0, -1 } /*mid left*/, Direction{  0, 1  } /*mid right*/,
        Direction{ 1, -1 } /*bottom left*/, Direction{ 1 , 0 }  /*mid bottom*/, Direction{ 1, 1 } /*bottom right*/,
    };

Grid parse_grid(std::ifstream& i_file)
{
    Grid grid;
    std::string line;
    while (std::getline(i_file, line))
    {
        grid.push_back(line);
    }
    return grid;
}

// Count rolls accessible

bool is_accessible(const Grid& i_grid, int i_pos_i, int i_pos_j, char i_target)
{
    const auto& max_row = i_grid.size();
    const auto& max_col = !i_grid.empty() ? i_grid.front().size() : 0;

    size_t count_rolls = std::count_if(
        DELTA_NEIGH.begin(),
        DELTA_NEIGH.end(),
        [&i_grid, &max_row, &max_col, &i_pos_i, &i_pos_j, &i_target](const auto& dir)
        {
            Direction pos{ i_pos_i + dir.first, i_pos_j + dir.second };
            return (pos.first >= 0 && pos.first < max_row
                && pos.second >= 0 && pos.second < max_col
                && i_grid.at(pos.first).at(pos.second) == i_target);
        }
    );
    return count_rolls < 4;
}

size_t count_accessible_rolls(const Grid& i_grid, char i_target = '@')
{
    int max_row = i_grid.size();
    int max_col = max_row != 0 ? i_grid.front().size() : 0;
    return std::ranges::distance(
        std::views::cartesian_product(std::views::iota(0, max_row), std::views::iota(0, max_col))
        | std::views::filter([&i_grid, &i_target](const auto& pair)
        {
            auto& [i, j] = pair;
            return i_grid.at(i).at(j) == i_target && is_accessible(i_grid, i, j, i_target);
        })
    );
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    Grid grid = parse_grid(file);
    std::cout << "Number of accessible rolls: " << count_accessible_rolls(grid) << std::endl;
    return 0;
}

// Count total of rolls that can be removed

size_t count_removable_rolls(const Grid& i_grid, char i_target = '@')
{
    Grid mutable_grid(i_grid.begin(), i_grid.end());
    size_t n_rolls = 0;
    size_t prev_n_rolls = 0;
    do
    {
        prev_n_rolls = n_rolls;
        for (int i : std::views::iota(0, static_cast<int>(mutable_grid.size())))
        {
            for (int j : std::views::iota(0, static_cast<int>(mutable_grid.at(i).size())))
            {
                if (mutable_grid.at(i).at(j) == i_target && is_accessible(mutable_grid, i, j, i_target))
                {
                    mutable_grid.at(i).at(j) = '.';
                    ++n_rolls;
                }
            }
    }
    } while(n_rolls > prev_n_rolls);
    return n_rolls;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    Grid grid = parse_grid(file);
    std::cout << "Number of removable rolls: " << count_removable_rolls(grid) << std::endl;
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
