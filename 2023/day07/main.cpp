#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <numeric>
#include <functional>
#include <exception>
#include <memory>
#include <utility>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 07 2023" << std::endl;
    std::cout << "Camel cards" << std::endl;
    std::cout << "Usage: day07_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

size_t score_from_two_largest_duplicates(
    const size_t& largest,
    const size_t& second_largest
){
    size_t check_sum = largest + second_largest;
    if(check_sum < 2 || check_sum > 5)
        throw std::runtime_error("Incorrect sum of cards: " + std::to_string(check_sum));

    // Link score to number of duplicates: times two to leave room for full house and two pairs
    size_t score = largest * 2;
    if(second_largest == 2) // full house or two pairs
        score++;

    return score;
}

struct Scorer{
    explicit Scorer(const std::unordered_map<char, size_t>& card_rank): m_card_rank(card_rank){}
    virtual size_t score_hand(const std::unordered_map<char, size_t>& cards_and_counts) = 0;

    std::unordered_map<char, size_t> m_card_rank;
};

struct HandScorer : public Scorer{
    explicit HandScorer(const std::unordered_map<char, size_t>& card_rank) : Scorer(card_rank){};
    size_t score_hand(const std::unordered_map<char, size_t>& cards_and_counts) override {
        // cannot sort map: extract vector
        std::vector<size_t> counters;
        std::transform(
            cards_and_counts.begin(),
            cards_and_counts.end(),
            std::back_inserter(counters),
            [](const auto& p){ return p.second; }
        );

        // Only two largest elements are enough
        std::nth_element(
            counters.begin(),
            std::next(counters.begin()),
            counters.end(),
            std::greater{}
        );

        size_t second_largest = counters.size() > 1 ? counters[1] : 0;
        size_t score = score_from_two_largest_duplicates(counters[0], counters[1]);

        return score;
    }
};

struct CamelHand{
    // Constructor
    CamelHand(const std::vector<size_t>& cards, const size_t& score): m_hand(cards), m_score(score){}

    CamelHand(
        const std::string& cards,
        std::unique_ptr<Scorer> const& ptr_scorer /*CamelHand does not own the ptr_scorer*/
        )
    {
        if(cards.length() != 5)
            throw std::runtime_error(
                "Invalid number of cards: " +
                std::to_string(cards.length()) +
                ", but 5 were expected!"
            );

        std::unordered_map<char, size_t> card_counter; // count duplicates

        for(const char& cc : cards){
            card_counter[cc]++;
            size_t card_value;
            if(std::isdigit(cc))
                card_value = static_cast<size_t>(cc - '0');
            else if(ptr_scorer->m_card_rank.count(cc) == 1)
                card_value = ptr_scorer->m_card_rank[cc];
            else
                card_value = 0;
            m_hand.push_back(card_value);
        }

        if(card_counter.empty())
            throw std::runtime_error("No count found from inputs cards: " + cards);

        m_score = ptr_scorer->score_hand(card_counter);
    }

    bool operator<(const CamelHand& rhs) const{
        bool is_lower;
        if(this->m_score != rhs.m_score)
            is_lower = this->m_score < rhs.m_score;
        // Call std::lexicographical_compare for explicit comparison
        else
            is_lower = std::lexicographical_compare(
                this->m_hand.begin(), this->m_hand.end(),
                rhs.m_hand.begin(), rhs.m_hand.end()
            );
        return is_lower;
    }

    // This is free
    bool operator>(const CamelHand& rhs) const{
        return rhs < *this;
    }

    private:
        std::vector<size_t> m_hand; // cards
        size_t m_score; // hand value/type
};

size_t sort_and_winnings(
    std::vector<std::pair<CamelHand, size_t>>& vec_hands_bids
){
    std::sort(
        vec_hands_bids.begin(),
        vec_hands_bids.end(),
        [](const auto& pl, const auto& pr){ return pl.first < pr.first; }
    );

    size_t winnings = 0;
    for(size_t i = 0, iend = vec_hands_bids.size(); i < iend; i++){
        winnings += (i + 1) * vec_hands_bids[i].second;
    }

    return winnings;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::string cards{};
    size_t bid{};
    std::vector<std::pair<CamelHand, size_t>> vec_hands_bids;
    std::unordered_map<char, size_t> card_rank = {
        {'T', 10},
        {'J', 11},
        {'Q', 12},
        {'K', 13},
        {'A', 14}
    };
    auto scorer = std::unique_ptr<Scorer>(new HandScorer(card_rank));
    while(file >> cards >> bid)
        vec_hands_bids.push_back(
            std::make_pair(
                CamelHand(cards, scorer),
                bid
            )
        );

    size_t winnings = sort_and_winnings(vec_hands_bids);

    std::cout << "Total winnings: " << winnings << std::endl;
    return 0;
}

typedef std::vector<std::pair<char, size_t>> CardCounters;

struct HandScorerJoker : public Scorer{
    explicit HandScorerJoker(const std::unordered_map<char, size_t>& card_rank) : Scorer(card_rank){};

    size_t score_hand(const std::unordered_map<char, size_t>& cards_and_counts) override {
        // cannot sort map: extract vector of pairs
        CardCounters counters(cards_and_counts.begin(), cards_and_counts.end());

        // Only three largest elements are enough
        std::nth_element(
            counters.begin(),
            std::next(counters.begin()),
            counters.end(),
            [](const auto& pl, const auto& pr){return pl.second > pr.second;}
        );

        auto it_joker = std::find_if(
            counters.begin(),
            counters.end(),
            [](const auto& p){ return p.first == 'J';}
        );

        auto it_largest = counters.begin();
        size_t largest = 0;
        size_t second_largest = 0;

        // Most cards are J, add them to largest and shift order
        if(it_largest == it_joker){
            largest = counters.size() > 1 ? counters[1].second + it_joker->second : it_joker->second;
            second_largest = counters.size() > 2 ? counters[2].second : 0;
        }
        else{
            if(it_joker != counters.end())
                it_largest->second += it_joker->second;

            auto it_second_largest = std::next(it_largest);
            if(it_second_largest == it_joker)
                it_second_largest++; // J was added to largest count, use third largest

            largest = it_largest->second;
            second_largest = it_second_largest != counters.end() ? it_second_largest->second : 0;
        }

        size_t score = score_from_two_largest_duplicates(largest,second_largest);
        return score;
    }
};

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    std::string cards{};
    size_t bid{};
    std::vector<std::pair<CamelHand, size_t>> vec_hands_bids;
    std::unordered_map<char, size_t> card_rank = {
        {'T', 10},
        {'J',  0},
        {'Q', 12},
        {'K', 13},
        {'A', 14}
    };
    auto scorer = std::unique_ptr<Scorer>(new HandScorerJoker(card_rank));
    while(file >> cards >> bid)
        vec_hands_bids.push_back(
            std::make_pair(
                CamelHand(cards, scorer),
                bid
        ));

    size_t winnings = sort_and_winnings(vec_hands_bids);

    std::cout << "Total winnings with Joker rule: " << winnings << std::endl;
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
