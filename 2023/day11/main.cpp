#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <utility>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <numeric>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 11 2023" << std::endl;
    std::cout << "Cosmic expansion" << std::endl;
    std::cout << "Usage: day11_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Aliases
typedef std::pair<size_t, size_t> GridLoc;
typedef unsigned long long ull;

void find_rows_cols_to_expand_from_galaxies(
    const std::vector<GridLoc>& loc_galaxies,
    const size_t& nrows,
    const size_t& ncols,
    std::vector<size_t>& rows_to_expand,
    std::vector<size_t>& cols_to_expand
){
    std::vector<bool> mask_rows(nrows, true);
    std::vector<bool> mask_cols(ncols, true);
    for(const auto& pp: loc_galaxies){
        if(pp.first >= nrows || pp.second >= ncols)
            throw std::runtime_error("Galaxy index overflow grid");

        mask_rows[pp.first] = false;
        mask_cols[pp.second] = false;
    }

    rows_to_expand.clear();
    for(size_t i = 0; i < nrows; i++)
        if(mask_rows[i]) rows_to_expand.push_back(i);

    cols_to_expand.clear();
    for(size_t i = 0; i < ncols; i++)
        if(mask_cols[i]) cols_to_expand.push_back(i);
}

void parse_galaxies(
    std::ifstream& file,
    std::vector<GridLoc>& loc_galaxies,
    std::vector<size_t>& rows_to_expand,
    std::vector<size_t>& cols_to_expand,
    const char& char_galaxy = '#'
){
    // Parsing
    loc_galaxies.clear(); // init vector of positions
    size_t row = 0;
    std::string line{};
    std::string last_line;
    while(std::getline(file, line)){
        size_t npos = 0;
        while((npos = line.find(char_galaxy, npos)) != std::string::npos){
            loc_galaxies.push_back(std::make_pair(row, npos));
            npos++;
        }
        row++;
        last_line = line;
    }

    // Finds rows cols to expand
    size_t nrows = row;
    size_t ncols = last_line.length();
    rows_to_expand.clear();
    cols_to_expand.clear();
    find_rows_cols_to_expand_from_galaxies(
        loc_galaxies,
        nrows,
        ncols,
        rows_to_expand,
        cols_to_expand
    );
}

size_t num_expdanded_steps(
    const size_t& min_val,
    const size_t& max_val,
    const std::vector<size_t>& ix_to_expand
){
    size_t nsteps = std::count_if(
        ix_to_expand.begin(),
        ix_to_expand.end(),
        [&min_val, &max_val](const size_t& ix){return (ix >= min_val) && (ix <= max_val);}
    );

    return nsteps;
}

std::vector<std::pair<GridLoc, GridLoc>> all_pairs_loc(
    const std::vector<GridLoc>& loc_galaxies
){
    std::vector<std::pair<GridLoc, GridLoc>> all_pairs;
    for(auto first_it = loc_galaxies.begin(); first_it != loc_galaxies.end(); first_it++){
        for(auto sec_it = std::next(first_it); sec_it != loc_galaxies.end(); sec_it++)
            all_pairs.push_back(make_pair(*first_it, *sec_it));
    }

    return all_pairs;
}

template<typename T>
T steps_for_path(
    const GridLoc& loc_start,
    const GridLoc& loc_end,
    const std::vector<size_t>& rows_to_expand,
    const std::vector<size_t>& cols_to_expand,
    const T& expand_width = 2
){
    size_t min_row = std::min(loc_start.first, loc_end.first);
    size_t max_row = std::max(loc_start.first, loc_end.first);
    size_t drow = max_row - min_row;
    size_t rows_to_add = num_expdanded_steps(min_row, max_row, rows_to_expand);

    size_t min_col = std::min(loc_start.second, loc_end.second);
    size_t max_col = std::max(loc_start.second, loc_end.second);
    size_t dcol = max_col - min_col;
    size_t cols_to_add = num_expdanded_steps(min_col, max_col, cols_to_expand);

    size_t nsteps = drow + dcol + (expand_width - 1) * (rows_to_add + cols_to_add);
    return nsteps;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::vector<GridLoc> loc_galaxies{};
    std::vector<size_t> rows_to_expand;
    std::vector<size_t> cols_to_expand;
    parse_galaxies(file, loc_galaxies, rows_to_expand, cols_to_expand);

    std::vector<std::pair<GridLoc, GridLoc>> all_pairs = all_pairs_loc(loc_galaxies);

    size_t sum_paths = 0;
    for(const auto& pp : all_pairs)
        sum_paths += steps_for_path<size_t>(pp.first, pp.second, rows_to_expand, cols_to_expand);

    std::cout << "Sum of shortest path steps: " << sum_paths << std::endl;
    return 0;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::vector<GridLoc> loc_galaxies{};
    std::vector<size_t> rows_to_expand;
    std::vector<size_t> cols_to_expand;
    parse_galaxies(file, loc_galaxies, rows_to_expand, cols_to_expand);

    std::vector<std::pair<GridLoc, GridLoc>> all_pairs = all_pairs_loc(loc_galaxies);

    ull sum_paths = 0;
    ull width_expansion = 1000000;
    for(const auto& pp : all_pairs)
        sum_paths += steps_for_path<ull>(pp.first, pp.second, rows_to_expand, cols_to_expand, width_expansion);

    std::cout << "Sum of shortest path steps: " << sum_paths << std::endl;
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
