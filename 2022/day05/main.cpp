#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <map>
#include <stack>
#include <vector>
#include <cctype>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 05 2022" << std::endl;
    std::cout << "Supply stacks" << std::endl;
    std::cout << "Usage: day05_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Init stacks from input lines
void init_stacks(
    const std::vector<std::string>& lines,
    std::map<std::string, std::stack<unsigned char>>& mmap
    )
{
    std::string str_keys = lines.back();
    std::map<std::string, int> keys_ix;
    int count = 0;
    std::stack<unsigned char> stack;

    for(const auto& c: str_keys)
    {
        if(!std::isspace(c))
        {
            keys_ix.insert(std::make_pair(std::string(1, c), count));
            mmap.insert(std::make_pair(std::string(1, c), stack));
        }
        
        count++;
    }

    for(auto it = lines.rbegin() + 1; it != lines.rend(); it++)
    {
        for(const auto& kix: keys_ix)
        {
            if(((*it).size() > kix.second) && ((*it)[kix.second] >= 'A'))
                mmap[kix.first].emplace(((*it)[kix.second]));
        }
    }
}

// Parse moving instructions
void parse_instructions(
    const std::string &inst,
    unsigned int &nmoves,
    std::string &stackfrom,
    std::string &stackto
    )
{
    std::vector<size_t> vecpos;
    size_t pos = 0;
    size_t tmp = 0;
    std::string delim = " ";
    
    while(pos < std::string::npos)
    {
        pos = inst.find(delim, tmp);
        tmp = pos + 1;
        vecpos.push_back(tmp);
    }

    nmoves = std::stoi(inst.substr(vecpos[0], vecpos[1] - vecpos[0] - 1));
    stackfrom = inst.substr(vecpos[2], vecpos[3] - vecpos[2] - 1);
    stackto = inst.substr(vecpos[4]);
}

// Move topn elements from one stack to another

void unstacking_elems(
    std::stack<unsigned char>& instack,
    std::stack<unsigned char>& outstack,
    unsigned int topn)
{
    unsigned int cnt = 0;
    
    while((!instack.empty()) && (cnt < topn))
    {
        outstack.push(instack.top());
        instack.pop();
        cnt++;
    }
}


// Follow stack instructions and get top stacks

int main_puzzle1(int argc, char** argv)
{   
    std::ifstream file(argv[1]);
    std::string line;
    std::vector<std::string> init_lines;

    // Init stacks

    while(std::getline(file, line) && (!line.empty()))
        init_lines.push_back(line);

    std::map<std::string, std::stack<unsigned char>> map_stacks;
    init_stacks(init_lines, map_stacks);

    // Parse and follow instructions

    unsigned int nmoves, cnt;
    std::string stackfrom, stackto;
    unsigned char elemtmp;

    while(std::getline(file, line))
    {
        parse_instructions(line, nmoves, stackfrom, stackto);
        unstacking_elems(map_stacks[stackfrom], map_stacks[stackto], nmoves);
    }

    std::cout << "[CrateMover 9000] Top stacks are: ";
    
    for(const auto& m: map_stacks)
        std::cout << m.second.top();
    
    std::cout << std::endl;
    
    return 0;
}

// MoveCrate 9001

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line;
    std::vector<std::string> init_lines;

    // Init stacks

    while(std::getline(file, line) && (!line.empty()))
        init_lines.push_back(line);

    std::map<std::string, std::stack<unsigned char>> map_stacks;
    init_stacks(init_lines, map_stacks);

    // Parse and follow instructions

    unsigned int nmoves;
    std::string stackfrom, stackto;
    unsigned char elemtmp;

    while(std::getline(file, line))
    {
        parse_instructions(line, nmoves, stackfrom, stackto);

        std::stack<unsigned char> tmp;
        unstacking_elems(map_stacks[stackfrom], tmp, nmoves);
        unstacking_elems(tmp, map_stacks[stackto], nmoves);
    }

    std::cout << "[CrateMover 9001] Top stacks are: ";
    
    for(const auto& m: map_stacks)
        std::cout << m.second.top();
    
    std::cout << std::endl;
    
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
