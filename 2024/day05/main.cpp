#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <deque>
#include <vector>
#include <utility>
#include <sstream>
#include <algorithm>
#include <iterator>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 05 2024" << std::endl;
    std::cout << "Print Queue" << std::endl;
    std::cout << "Usage: day05_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

/// Rules for ordering pages: vector of pairs where first comes before second
using OrderRules = std::deque<std::pair<long, long>>;

/// Parse inputs
void parse_rules_and_pages(std::ifstream& i_file,
   OrderRules& o_rules,
   std::vector<std::vector<long>>& o_pages)
{
    // parse rules
    o_rules.clear();
    std::string line;

    while(std::getline(i_file, line)){
        size_t pos_delim = line.find("|");
        if (pos_delim == std::string::npos)
            break;
        o_rules.push_back(
            std::make_pair(
                std::stol(line.substr(0, pos_delim)),
                std::stol(line.substr(pos_delim + 1))
            )
        );
    }

    // parse pages
    o_pages.clear();
    while(std::getline(i_file, line)){
        std::stringstream stream_line{ line };
        std::string str_num;
        std::vector<long> page_set;
        while(std::getline(stream_line, str_num, ','))
            page_set.push_back(std::stol(str_num));
        o_pages.push_back(page_set);
    }
}

/// Get middle element among pages
template <typename Iterator>
Iterator middle(Iterator first, Iterator last){
    size_t n_to_middle = std::distance(first, last) / 2;
    return std::next(first, n_to_middle);
}

/// Does rule apply to pages
template <typename Iterator>
bool does_rule_apply(
    Iterator first,
    Iterator last,
    long first_value,
    long second_value,
    Iterator& o_first_value_found,
    Iterator& o_second_value_found
)
{
    o_first_value_found = std::find(first, last, first_value);
    o_second_value_found = std::find(first, last, second_value);
    return (o_first_value_found == last
        || o_second_value_found == last
        || std::distance(o_first_value_found, o_second_value_found) >= 0);
}

/// Sum of correct middle pages: brute-forcing all rules
long sum_correct_middle_pages(
    const OrderRules& i_rules,
    const std::vector<std::vector<long>>& i_all_pages)
{
    long sum_middle = 0;
    for (const auto& pages : i_all_pages){
        auto first_found = pages.begin();
        auto second_found = pages.begin();
        if (std::all_of(
                i_rules.begin(),
                i_rules.end(),
                [&pages, &first_found, &second_found](const auto& i_pair){
                    return does_rule_apply(
                        pages.begin(),
                        pages.end(),
                        i_pair.first,
                        i_pair.second,
                        first_found,
                        second_found);
                })
            )
            sum_middle += *middle(pages.begin(), pages.end());
    }
    return sum_middle;
}

// Sum of correct middle page number
int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    OrderRules rules;
    std::vector<std::vector<long>> pages;
    parse_rules_and_pages(file, rules, pages);
    long sum_middle = sum_correct_middle_pages(rules, pages);
    std::cout << "Sum of correct middle pages: " << sum_middle << std::endl;
    return 0;
}

/// Fix pages order and sum these middle pages
long sum_fixed_middle_pages(
    const OrderRules& i_rules,
    const std::vector<std::vector<long>>& i_all_pages)
{
    long sum_middle = 0;
    for (auto pages : i_all_pages){
        bool is_fixed = false;
        for(auto it_rule = i_rules.begin(); it_rule != i_rules.end(); ++it_rule){
            auto first_found = pages.begin();
            auto second_found = pages.begin();
            if (!does_rule_apply(pages.begin(), pages.end(), it_rule->first, it_rule->second, first_found, second_found)){
                // swap until fixed order
                std::iter_swap(first_found, second_found);
                is_fixed = true;
                it_rule = i_rules.begin(); // restart checking rules
            }
        }
        if (is_fixed)
            sum_middle += *middle(pages.begin(), pages.end());
    }
    return sum_middle;
}

// Fix and sum middle pages
int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    OrderRules rules;
    std::vector<std::vector<long>> pages;
    parse_rules_and_pages(file, rules, pages);
    long sum_middle = sum_fixed_middle_pages(rules, pages);
    std::cout << "Sum with fixed middle pages: " << sum_middle << std::endl;
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
