#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <array>
#include <vector>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 04 2024" << std::endl;
    std::cout << "Ceres Search" << std::endl;
    std::cout << "Usage: day04_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Find XMAS count in grid

/// Parse input into 2D grid of char
std::vector<std::string> parse_grid(std::ifstream& i_file)
{
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(i_file, line))
        grid.push_back(line);

    return grid;
}

/// Check from (row, col) position if subtrig is equal to target in (delta_row, delta_col) direction.
bool is_equal_to_target(
    const std::vector<std::string>& i_grid,
    const size_t i_n_rows,
    const size_t i_n_cols,
    const std::string& i_target,
    size_t i_row,
    size_t i_col,
    const int i_delta_row,
    const int i_delta_col
)
{
    for (char w : i_target){
        if (i_row >= i_n_rows || i_col >= i_n_cols)
            return false;

        if (i_grid[i_row][i_col] != w)
            return false;

        i_row += i_delta_row;
        i_col += i_delta_col;
    }
    return true;
}

/// Count occurences of target in minimal grid
long count_target_grid(
    const std::vector<std::string>& i_grid,
    const std::string& i_target = "XMAS")
{
    long n_counts_target = 0;
    if (i_grid.empty())
        return n_counts_target;

    size_t max_rows = i_grid.size();
    size_t max_cols = i_grid.front().size();
    std::array<int, 3> deltas = {-1, 0, 1};

    for (size_t i = 0; i < max_rows; ++i){
        for (size_t j = 0; j < max_cols; j++){
            for (auto di : deltas){
                for (auto dj : deltas)
                {
                    if (di == 0 && dj == 0)
                        continue;
                    n_counts_target += is_equal_to_target(
                        i_grid, max_rows, max_cols,
                        i_target,
                        i, j, di, dj
                    );
                }
            }
        }
    }


    return n_counts_target;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::string> grid = parse_grid(file);
    std::string target = "XMAS";
    long n_counts_target = count_target_grid(grid, target);
    std::cout << "#Occurences of " << target << ": " << n_counts_target << std::endl;
    return 0;
}

// Find X-MAS count in grid

/// Check if characters describe X-MAS shape
bool is_x_mas_shape(
    char i_topleft,
    char i_topright,
    char i_middle,
    char i_botleft,
    char i_botright
)
{
    if (i_middle != 'A')
        return false;

    bool first_diag = (
        i_topleft == 'S' && i_botright == 'M'
        || i_topleft == 'M' && i_botright == 'S'
    );

    bool second_diag = (
        i_topright == 'S' && i_botleft == 'M'
        || i_topright == 'M' && i_botleft == 'S'
    );

    return first_diag && second_diag;
}

/// Count X-MAS pattern in grid
long count_x_mas_pattern(const std::vector<std::string>& i_grid)
{
    long n_x_mas_pattern = 0;
    if (i_grid.empty())
        return n_x_mas_pattern;
    size_t max_cols = i_grid.front().size();

    for(auto pi = 0, ci = 1, ni = 2; ni < i_grid.size(); ++pi, ++ci, ++ni){
        for (auto pj = 0, cj = 1, nj = 2; nj < max_cols; ++pj, ++cj, ++nj){
            if (is_x_mas_shape(
                i_grid[pi][pj], // topleft
                i_grid[pi][nj], // top right
                i_grid[ci][cj], // middle
                i_grid[ni][pj], // bottom left
                i_grid[ni][nj] // bottom right
            ))
                n_x_mas_pattern++;
        }
    }

    return n_x_mas_pattern;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::string> grid = parse_grid(file);
    long n_counts_target = count_x_mas_pattern(grid);
    std::cout << "#Occurences of X-MAS pattern: " << n_counts_target << std::endl;
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
