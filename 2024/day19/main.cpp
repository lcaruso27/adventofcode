#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 19 2024" << std::endl;
    std::cout << "Linen Layout" << std::endl;
    std::cout << "Usage: day19_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

/// Parse towels and designs
void parse_towels_designs(
    std::ifstream& io_file,
    std::vector<std::string>& o_towels,
    std::vector<std::string>& o_designs,
    const std::string& i_towel_delim = ", "
)
{
    o_towels.clear();
    o_designs.clear();
    { // towels
        std::string line;
        std::getline(io_file, line);
        if (!io_file.good())
            return;

        size_t size_delim = i_towel_delim.size();
        size_t pos = 0;
        for (
            size_t found = line.find(i_towel_delim);
            found != std::string::npos;
            pos = size_delim + found,
            found = line.find(i_towel_delim, pos)
        )
            o_towels.push_back(line.substr(pos, found - pos));
        o_towels.push_back(line.substr(pos));
    }
    { // designs
        std::string design;
        while (std::getline(io_file, design)){
            if (design.empty())
                continue;
            o_designs.push_back(design);
        }
    }
}

/// Count number of possible arrangements for a design
/// Using recursive and memoization.
long count_arrangements(
    const std::vector<std::string>& i_towels,
    const std::string& i_design,
    std::unordered_map<std::string, long>& io_memo
)
{
    if (i_design.empty())
        return 1l;

    if (io_memo.find(i_design) != io_memo.end())
        return io_memo[i_design];

    long total = 0;
    for (const auto& towel : i_towels){
        size_t nchars = towel.size();

        if (i_design.substr(0, nchars) == towel)
            total += count_arrangements(i_towels, i_design.substr(nchars), io_memo);
    }

    io_memo[i_design] = total;
    return total;
}

/// Count of possible designs
int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::string> towels, designs;
    parse_towels_designs(file, towels, designs);
    std::unordered_map<std::string, long> memo;
    long count_designs = std::count_if(
        designs.begin(),
        designs.end(),
        [&towels, &memo](const auto& design) { return count_arrangements(towels, design, memo) > 0; }
    );
    std::cout << "#Possible designs: " << count_designs << std::endl;
    return 0;
}

/// Sum of possible design counts
int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<std::string> towels, designs;
    parse_towels_designs(file, towels, designs);
    std::unordered_map<std::string, long> memo;
    long total = 0;
    std::for_each(
        designs.begin(),
        designs.end(),
        [&towels, &memo, &total](const auto& design) { total += count_arrangements(towels, design, memo); }
    );
    std::cout << "Sum of possible designs: " << total << std::endl;
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
