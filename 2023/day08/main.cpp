#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <unordered_map>
#include <numeric>
#include <exception>
#include <vector>
#include <iterator>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 08 2023" << std::endl;
    std::cout << "Wasteland path" << std::endl;
    std::cout << "Usage: day08_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Aliases
typedef std::pair<std::string, std::string> DirectionsLoc;
typedef std::unordered_map<std::string, DirectionsLoc> MapLoc;

void add_to_map(
    const std::string& line,
    MapLoc& mapland
){
    // Let's hardcoding positions
    size_t count_loc = 3;
    size_t pos_loc = 0;
    std::string val_loc = line.substr(pos_loc, count_loc);

    size_t pos_loc_left = 7;
    std::string val_loc_left = line.substr(pos_loc_left, count_loc);

    size_t pos_loc_right = 12;
    std::string val_loc_right = line.substr(pos_loc_right, count_loc);

    mapland[val_loc] = std::make_pair(val_loc_left, val_loc_right);
}

bool parse_input_file(
    std::ifstream& file,
    std::string& directions,
    MapLoc& mapland
){
    if(!std::getline(file, directions))
        return false;

    // parse empty line
    std::string line{};
    if(!std::getline(file, line))
        return false;

    while(std::getline(file, line))
        add_to_map(line, mapland);

    return true;
}

void follow_direction_inplace(
    std::string& start,
    const DirectionsLoc& choice,
    const char& direction
){
    switch(direction){
        case 'L':
            start = choice.first;
            break;
        case 'R':
            start = choice.second;
            break;
        default:
            throw std::runtime_error("unknow direction: " + std::string{direction});
    }
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::string path_directions;
    MapLoc mapland{};

    if(!parse_input_file(file, path_directions, mapland)){
        std::cerr << "cannot parse input file: " << argv[1] << std::endl;
        return 1;
    }

    std::string start = "AAA";
    std::string target = "ZZZ";
    size_t nsteps = 0;
    size_t ix_directions = 0;
    size_t num_directions = path_directions.length();
    while(start != target){
        while(ix_directions < num_directions){
            follow_direction_inplace(start, mapland[start], path_directions[ix_directions]);
            ix_directions++;
            nsteps++;
        }
        ix_directions = 0; // restart loop over directions
    }

    std::cout << "Steps required to go to " << target << ": " << nsteps << std::endl;
    return 0;
}

// Thanks reddit for the solution: lcm between all num steps
size_t gcd(const size_t& left, const size_t& right){
    if(left == 0)
        return right;
    if(right == 0)
        return left;

    if(left < right)
        return gcd(right % left, left);

    return gcd(left % right, right);
}

size_t lcm(const size_t& left, const size_t& right){
    return left * right / gcd(left, right);
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::string path_directions{};
    MapLoc mapland;

    if(!parse_input_file(file, path_directions, mapland)){
        std::cerr << "cannot parse input file: " << argv[1] << std::endl;
        return 1;
    }

    // Retrieve start locations
    std::vector<std::string> all_starts{};
    for(auto it = mapland.begin(); it != mapland.end(); it++)
        if(it->first.back() == 'A')
            all_starts.push_back(it->first);

    std::vector<size_t> all_num_steps{};
    size_t num_directions = path_directions.length();
    for(std::string& start : all_starts){
        size_t ix_directions = 0;
        size_t nsteps = 0;
        while(start.back() != 'Z'){
            while(ix_directions < num_directions){
                follow_direction_inplace(start, mapland[start], path_directions[ix_directions]);
                nsteps++;
                ix_directions++;
            }
            ix_directions = 0; // restart loop over directions
        }
        all_num_steps.push_back(nsteps);
    }

    size_t lcm_steps = std::accumulate(
        std::next(all_num_steps.begin()),
        all_num_steps.end(),
        all_num_steps.front(),
        lcm
    );
    std::cout << "Steps required to reach all targets: " << lcm_steps << std::endl;
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
