#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 04 2023" << std::endl;
    std::cout << "Scratchcards" << std::endl;
    std::cout << "Usage: day04_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

std::vector<size_t> parse_numbers(std::stringstream line){
    std::vector<size_t> numbers{};
    std::string num{};
    while(std::getline(line, num, ' ')){
        if(num.empty())
            continue;

        numbers.push_back(std::stoi(num));
    }

    return numbers;
}

size_t card_score(
    const std::vector<size_t>& winning_num,
    const std::vector<size_t>& candidates
){
    float points = 0.5; // first point = 1, then doubles
    float increment = 2;

    for(const size_t& num : candidates){
        auto find_winner = std::find(winning_num.begin(), winning_num.end(), num);
        if(find_winner != winning_num.end())
            points *= increment;
    }

    return static_cast<size_t>(points);
}

size_t parse_line_and_score(
    const std::string& line,
    const size_t& offset_winning,
    const size_t& count_winning,
    const size_t& offset_cdtes
){
    std::vector<size_t> num_winning = parse_numbers(
        std::stringstream{line.substr(offset_winning, count_winning)}
    );

    std::vector<size_t> candidates = parse_numbers(
        std::stringstream{line.substr(offset_cdtes)}
    );

    size_t score = card_score(num_winning, candidates);
    return score;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    size_t sum_pts = 0;
    std::string line{};
    std::string card_delim = ": ";
    std::string cdtes_delim = " | ";

    // First call to get delim pos
    if(!std::getline(file, line))
        return 1;

    size_t offset_winning = line.find(card_delim) + card_delim.length();
    size_t pos_cdtes = line.find(cdtes_delim);
    size_t count_winning = pos_cdtes - offset_winning;
    size_t offset_cdtes = pos_cdtes + cdtes_delim.length();

    sum_pts += parse_line_and_score(
        line,
        offset_winning,
        count_winning,
        offset_cdtes
    );

    // While loop for over lines
    while(std::getline(file, line))
        sum_pts += parse_line_and_score(
            line,
            offset_winning,
            count_winning,
            offset_cdtes
        );

    std::cout << "Sum of card scores: " << sum_pts << std::endl;
    return 0;
}

size_t num_matching(
    const std::vector<size_t>& winning_num,
    const std::vector<size_t>& candidates
){
    size_t nwins = 0;
    for(const size_t& num : candidates){
        auto find_winner = std::find(winning_num.begin(), winning_num.end(), num);
        if(find_winner != winning_num.end())
            nwins++;
    }

    return nwins;
}

void update_ncards(
    const size_t& ix_card,
    const size_t& n_wins,
    std::vector<size_t>& instances
){
    size_t last_ix_card = ix_card + n_wins;
    bool is_long_enough = last_ix_card < instances.size();
    size_t n_cards_to_inc = is_long_enough ? last_ix_card : instances.size() - 1;
    for(size_t i = ix_card + 1; i <= n_cards_to_inc; i++)
        instances[i]++;

    size_t remaining_cards = 0;
    if(!is_long_enough)
        remaining_cards = last_ix_card - instances.size() + 1;

    for(size_t i = 0; i < remaining_cards; i++)
        instances.push_back(1);
}

void parse_line_and_update(
    const std::string& line,
    const size_t& offset_winning,
    const size_t& count_winning,
    const size_t& offset_cdtes,
    const size_t ix_card,
    std::vector<size_t>& instances
){
    if(ix_card < instances.size())
        instances[ix_card]++;
    else
        instances.push_back(1);

    std::vector<size_t> num_winning = parse_numbers(
        std::stringstream{line.substr(offset_winning, count_winning)}
    );

    std::vector<size_t> candidates = parse_numbers(
        std::stringstream{line.substr(offset_cdtes)}
    );
    size_t n_matches = num_matching(num_winning, candidates);

    for(size_t i = 0; i < instances[ix_card]; i++)
        update_ncards(ix_card, n_matches, instances);
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::vector<size_t> all_instances;
    size_t ix_card = 0;
    std::string line{};
    std::string card_delim = ": ";
    std::string cdtes_delim = " | ";

    // First call to get delim pos
    if(!std::getline(file, line))
        return 1;

    size_t offset_winning = line.find(card_delim) + card_delim.length();
    size_t pos_cdtes = line.find(cdtes_delim);
    size_t count_winning = pos_cdtes - offset_winning;
    size_t offset_cdtes = pos_cdtes + cdtes_delim.length();

    parse_line_and_update(
        line,
        offset_winning,
        count_winning,
        offset_cdtes,
        ix_card,
        all_instances
    );
    ix_card++;

    while(std::getline(file, line)){
        parse_line_and_update(
            line,
            offset_winning,
            count_winning,
            offset_cdtes,
            ix_card,
            all_instances
        );

        ix_card++;
    }

    size_t sum = std::accumulate(all_instances.begin(), all_instances.end(), 0);
    std::cout << "Total of scratchcards won: " << sum << std::endl;
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
