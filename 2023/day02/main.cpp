#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 02 2023" << std::endl;
    std::cout << "Cube game" << std::endl;
    std::cout << "Usage: day02_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Utils

std::vector<std::string> tokenize(
    const std::string& line,
    const std::string& delim
){
    std::vector<std::string> tokens{};
    size_t start_pos = 0, end_pos = 0;
    while(end_pos != std::string::npos){
        end_pos = line.find(delim, start_pos);
        size_t count = end_pos != std::string::npos ? end_pos - start_pos : std::string::npos;
        tokens.push_back(line.substr(start_pos, count));

        start_pos = end_pos + delim.length();
    }

    return tokens;
}

// Struct to represent elements of the puzzle

// Cube sample
struct CubeSample{
    std::map<std::string, size_t> m_sample = {
        {"red", 0},
        {"green", 0},
        {"blue", 0},
    };
    CubeSample(){}
    explicit CubeSample(const std::map<std::string, size_t>& sample) : m_sample(sample){}

    void update_from_str_drawn(const std::string& drawn){
        for(auto it = m_sample.begin(); it != m_sample.end(); ++it)
        {
            size_t npos = drawn.find(it->first);
            if(npos == std::string::npos)
                continue;

            it->second = std::stoi(drawn.substr(0, npos - 1));
        }
    }

    bool parse(const std::string& sample_desc){
        std::vector<std::string> split_drawn = tokenize(sample_desc, ", ");
        if(split_drawn.empty())
            return false;

        for(const std::string& drawn : split_drawn)
            update_from_str_drawn(drawn);

        return true;
    }

    bool is_from(const CubeSample& from) const{
        return (m_sample.at("red") <= from.m_sample.at("red")) &&
            (m_sample.at("green") <= from.m_sample.at("green")) &&
            (m_sample.at("blue") <= from.m_sample.at("blue"));
    }

    unsigned int power() const{
        unsigned int power = 1;
        for(auto it = m_sample.begin(); it != m_sample.end(); ++it)
            power *= it->second;

        return power;
    }
};

// Game
struct CubeGame{
    unsigned int m_id = 0;
    std::vector<CubeSample> m_samples;

    CubeGame(){};

    bool parse(const std::string& game_desc)
    {
        // Find game id
        std::string game_id_start_delim = "Game ";
        std::string game_id_end_delim = ": ";

        auto pos_start_id = game_id_start_delim.length();
        auto pos_end_id = game_desc.find(game_id_end_delim);
        if(pos_end_id == std::string::npos)
            return false;

        m_id = std::stoi(game_desc.substr(pos_start_id, pos_end_id));

        // Parse samples
        std::string all_samples_desc = game_desc.substr(pos_end_id + game_id_end_delim.length());
        std::vector<std::string> cube_samples = tokenize(all_samples_desc, "; ");
        if(cube_samples.empty())
            return false;

        for(const std::string& sample : cube_samples){
            CubeSample cube;
            if(!cube.parse(sample))
                return false;

            m_samples.push_back(cube);
        }

        return true;
    }

    bool is_possible(const CubeSample& ref_bag) const{
        return std::all_of(
            m_samples.begin(),
            m_samples.end(),
            [&ref_bag](const CubeSample& drawn){return drawn.is_from(ref_bag);});
    }

    CubeSample min_cube() const{
        CubeSample min_cube{};
        for(auto it = min_cube.m_sample.begin(); it != min_cube.m_sample.end(); ++it)
        {
           auto it_min = std::max_element(
                m_samples.begin(),
                m_samples.end(),
                [&](const auto& lhs, const auto& rhs){
                    return lhs.m_sample.at(it->first) < rhs.m_sample.at(it->first);
                    });

            it->second = it_min->m_sample.at(it->first);
        }

        return min_cube;
    }
};

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    int sum_ids = 0;
    CubeSample cube_bag{{{"red", 12}, {"green", 13}, {"blue", 14}}};

    std::string line;
    while(std::getline(file, line)){
        CubeGame game;
        if(!game.parse(line))
            continue;

        if(game.is_possible(cube_bag))
            sum_ids += game.m_id;
    }
    std::cout << "Sum of possible games: " << sum_ids << std::endl;
    return 0;
}

// Comment

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    int sum_powers = 0;
    CubeSample cube_bag{{{"red", 12}, {"green", 13}, {"blue", 14}}};

    std::string line;
    while(std::getline(file, line)){
        CubeGame game;
        if(!game.parse(line))
            continue;

        CubeSample min_cube = game.min_cube();
        sum_powers += min_cube.power();

    }
    std::cout << "Sum of powers: " << sum_powers << std::endl;
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
