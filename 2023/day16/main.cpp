#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <deque>
#include <iterator>
#include <numeric>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 16 2023" << std::endl;
    std::cout << "The floor will be lava" << std::endl;
    std::cout << "Usage: day16_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Grid directions
enum class Direction{
    Up,
    Down,
    Left,
    Right,
    Undefined
};

std::string str_direction(const Direction& direction){
    std::string str_dir{};
    switch(direction){
        case Direction::Up:
            str_dir = "up";
            break;
        case Direction::Down:
            str_dir = "down";
            break;
        case Direction::Left:
            str_dir = "left";
            break;
        case Direction::Right:
            str_dir = "right";
            break;
        default:
            str_dir = "undefined";
            break;
    }
    return str_dir;
}

// Grid position instruction
struct GridLoc{
    int m_row = -1;
    int m_col = -1;
    Direction m_direction = Direction::Undefined;

    GridLoc(const int& row, const int& col, const Direction& dir) : m_row(row), m_col(col), m_direction(dir){}
};

// Print GridLoc for debug
std::ostream& operator<<(std::ostream& os, const GridLoc& pos){
        return os << "[" << pos.m_row << ", " << pos.m_col << ", " << str_direction(pos.m_direction) << "]";
    }

// Update position from given direction
void follow_direction(GridLoc& pos){
    switch(pos.m_direction){
        case Direction::Up:
            pos.m_row--;
            break;
        case Direction::Down:
            pos.m_row++;
            break;
        case Direction::Left:
            pos.m_col--;
            break;
        case Direction::Right:
            pos.m_col++;
            break;
        default:
            throw std::runtime_error("Invalid direction");
    }
}

// Mirror function
void mirror(
    std::deque<GridLoc>& path,
    bool positive
){
    auto it = std::prev(path.end()); // last element: modify it in place, no need to pop

    Direction mirror_dir = Direction::Undefined;
    // Update direction
    switch(it->m_direction){
        case Direction::Up:
            mirror_dir = positive ? Direction::Right : Direction::Left;
            break;
        case Direction::Down:
            mirror_dir = positive ? Direction::Left : Direction::Right;
            break;
        case Direction::Left:
            mirror_dir = positive ? Direction::Down : Direction::Up;
            break;
        case Direction::Right:
            mirror_dir = positive ? Direction::Up : Direction::Down;
            break;
        default:
            throw std::runtime_error("Invalid direction");
    }

    // Replace last element in place: equiv to back() + pop_back() + push_back() here
    it->m_direction = mirror_dir;
}

// Splitting beam
void splitter(
    std::deque<GridLoc>& path,
    bool vertical
){
    GridLoc pos = path.back();
    if(vertical){
        switch(pos.m_direction){
            case Direction::Left:
            case Direction::Right:
                path.pop_back();
                path.push_back(GridLoc(pos.m_row, pos.m_col, Direction::Up));
                path.push_back(GridLoc(pos.m_row, pos.m_col, Direction::Down));
                break;
            case Direction::Up:
            case Direction::Down:
                break; // do nothing
            default:
                std::runtime_error("Invalid direction");
        }
    }
    else{
        switch(pos.m_direction){
            case Direction::Up:
            case Direction::Down:
                path.pop_back();
                path.push_back(GridLoc(pos.m_row, pos.m_col, Direction::Left));
                path.push_back(GridLoc(pos.m_row, pos.m_col, Direction::Right));
                break;
            case Direction::Left:
            case Direction::Right:
                break; // do nothing
            default:
                std::runtime_error("Invalid error");
        }
    }
}

// Wrapper around functions to update both path and visited
void next_pos(
    const char& step,
    std::deque<GridLoc>& path,
    std::deque<std::deque<Direction>>& visited
){
    auto it = std::prev(path.end());
    visited[it->m_row][it->m_col] = it->m_direction;

    // Update path
    switch(step){
        case '/':
            mirror(path, true /*+90 degrees*/);
            break;
        case '\\':
            mirror(path, false /*-90 degrees*/);
            break;
        case '|':
            splitter(path, true /*vertical*/);
            break;
        case '-':
            splitter(path, false /*horizontal*/);
            break;
        case '.':
            // do nothing
            break;
        default:
            throw std::runtime_error("Invalid step found: " + std::string{step});
    }
    follow_direction(*it);
}

// Recursive function to explore the grid
void grid_beam(
    const std::deque<std::string>& grid,
    std::deque<GridLoc>& path,
    std::deque<std::deque<Direction>>& visited,
    const size_t& lim_rows,
    const size_t& lim_cols
){
    if(path.empty())
        return;

    GridLoc pos = path.back();

    // outside
    if((pos.m_row < 0) || (pos.m_row > lim_rows) || (pos.m_col < 0) || (pos.m_col > lim_cols))
        path.pop_back();
    // cycle
    else if(visited[pos.m_row][pos.m_col] == pos.m_direction)
        path.pop_back();
    else
        next_pos(grid[pos.m_row][pos.m_col], path, visited);

    grid_beam(grid, path, visited, lim_rows, lim_cols);
}

// Compute number of energized tiles
size_t num_energized(const std::deque<std::deque<Direction>>& visited){
    size_t ntiles = 0;
    for(const auto& columns : visited)
        ntiles += std::count_if(
            columns.begin(),
            columns.end(),
            [](const auto& dir){ return dir != Direction::Undefined; }
        );
    return ntiles;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line;
    std::deque<std::string> grid;
    std::deque<std::deque<Direction>> visited;
    while(file >> line){
        grid.push_back(line);
        visited.push_back(std::deque<Direction>(line.length(), Direction::Undefined));
    }

    int lim_rows = grid.size() - 1;
    int lim_cols = grid.front().length() - 1;
    std::deque<GridLoc> path = {GridLoc(0, 0, Direction::Right)};
    grid_beam(grid, path, visited, lim_rows, lim_cols);

    size_t ntiles = num_energized(visited);

    std::cout << "Number of energized tiles: " << ntiles << std::endl;
    return 0;
}

std::deque<GridLoc> find_start_points(
    const int& lim_rows,
    const int& lim_cols
){
    std::deque<GridLoc> start_points{
        GridLoc(0, 0, Direction::Right),
        GridLoc(0, lim_cols, Direction::Left),
        GridLoc(lim_rows, 0, Direction::Right),
        GridLoc(lim_rows, lim_cols, Direction::Left),
    };
    for(size_t i = 0; i <= lim_cols; i++){
        start_points.push_back(GridLoc(0, i, Direction::Down));
        start_points.push_back(GridLoc(i, lim_rows, Direction::Up));
    }
    for(size_t i = 0; i <= lim_rows; i++){
        start_points.push_back(GridLoc(i, 0, Direction::Right));
        start_points.push_back(GridLoc(i, lim_cols, Direction::Left));
    }

    return start_points;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line;
    std::deque<std::string> grid;
    std::deque<std::deque<Direction>> visited;
    while(file >> line){
        grid.push_back(line);
        visited.push_back(std::deque<Direction>(line.length(), Direction::Undefined));
    }

    int lim_rows = grid.size() - 1;
    int lim_cols = grid.front().length() - 1;

    size_t max_ntiles = 0;
    std::deque<GridLoc> start_points = find_start_points(lim_rows, lim_cols);
    for(const auto& start : start_points){
        std::deque<GridLoc> path{start};
        std::deque<std::deque<Direction>> loop_visited = visited;
        grid_beam(grid, path, loop_visited, lim_rows, lim_cols);
        size_t ntiles = num_energized(loop_visited);

        if(ntiles >= max_ntiles)
            max_ntiles = ntiles;
    }
    std::cout << "Max number of energized tiles: " << max_ntiles << std::endl;
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
