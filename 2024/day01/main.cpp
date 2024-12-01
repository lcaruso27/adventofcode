#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 01 2024" << std::endl;
    std::cout << "Pairing lists" << std::endl;
    std::cout << "Usage: day01_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Distance sum between each pairs

/// Parse inputs into vectors of ids: left and right ones.
void parse_input_lists(
    std::ifstream& io_file,
    std::vector<long>& o_left_ids,
    std::vector<long>& o_right_ids)
{
    o_left_ids.clear();
    o_right_ids.clear();

    long left_id = 0, right_id = 0;
    while (io_file >> left_id >> right_id){
        o_left_ids.push_back(left_id);
        o_right_ids.push_back(right_id);
    }
}

/// Distance between paired ids.
std::vector<long> distance_id_pairs(std::vector<long>& io_left_ids, std::vector<long>& io_right_ids)
{
    // Pairing
    std::sort(io_left_ids.begin(), io_left_ids.end());
    std::sort(io_right_ids.begin(), io_right_ids.end());
    std::vector<long> distance_paired_ids(io_left_ids.size());
    std::transform(
        io_left_ids.begin(),
        io_left_ids.end(),
        io_right_ids.begin(),
        distance_paired_ids.begin(),
        [](const auto& i_left_id, const auto& i_right_id) { return std::abs(i_left_id - i_right_id); }
        );

    return distance_paired_ids;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<long> left_ids;
    std::vector<long> right_ids;
    parse_input_lists(file, left_ids, right_ids);

    auto distance_paired_ids = distance_id_pairs(left_ids, right_ids);
    auto sum_distances = std::accumulate(distance_paired_ids.begin(), distance_paired_ids.end(), 0 /*init*/);

    std::cout << "Sum paired ids distances: " << sum_distances << std::endl;
    return 0;
}

// Similarity score between lists

/// Similarity scores for each left id
std::vector<long> similarity_scores(const std::vector<long>& i_left_ids, const std::vector<long>& i_right_ids)
{
    // Store similarity score coefficient per id (histogram)
    long max_left_id = *std::max_element(i_left_ids.begin(), i_left_ids.end());
    std::vector<long> coefficients(max_left_id + 1);
    std::for_each(
        i_left_ids.begin(),
        i_left_ids.end(),
        [&coefficients, &i_right_ids]
        (const auto& i_value){ coefficients[i_value] = std::count(
            i_right_ids.begin(),
            i_right_ids.end(),
            i_value);
            }
    );

    // Compute similarity scores
    std::vector<long> scores(i_left_ids.size());
    std::transform(
        i_left_ids.begin(),
        i_left_ids.end(),
        scores.begin(),
        [&coefficients](const auto& i_value){ return i_value * coefficients[i_value];}
    );

    return scores;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::vector<long> left_ids;
    std::vector<long> right_ids;
    parse_input_lists(file, left_ids, right_ids);

    auto distance_paired_ids = similarity_scores(left_ids, right_ids);
    auto sum_similarity_scores = std::accumulate(distance_paired_ids.begin(), distance_paired_ids.end(), 0 /*init*/);

    std::cout << "Similarity score: " << sum_similarity_scores << std::endl;
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
