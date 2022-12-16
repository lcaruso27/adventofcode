#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 11 2022" << std::endl;
    std::cout << "Monkey in the middle" << std::endl;
    std::cout << "Usage: day11_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Level of monkey business after 20 rounds

// Embedding monkey behaviour in class

class Monkey
{
    private:
        std::deque<unsigned long> m_items;
        int m_numinspect;
        int m_divider;
        int m_worry_divider;
        std::function<unsigned long(unsigned long, unsigned long)> m_update;

    public:
        // Constructors
        Monkey(){}

        Monkey
        (
            const int& numinspect,
            const int& divider,
            const int& worry_div,
            const std::function<unsigned long(unsigned long, unsigned long)> fun
        ):  m_numinspect(numinspect),
            m_divider(divider),
            m_worry_divider(worry_div),
            m_update(fun) {}

        // Destructors
        ~Monkey(){}

        // Getter
        int get_num_inspect(){return this->m_numinspect;}
        unsigned long get_item();
        bool is_empty(){return this->m_items.empty();}
        int get_divider(){return this->m_divider;}

        // Setter
        void add_item(const int& item){this->m_items.push_back(item);}

        // Inspect item
        bool inspect(unsigned long item, unsigned long lcm, unsigned long& newvalue);
};

unsigned long Monkey::get_item()
{
    unsigned long item = this->m_items.front();
    this->m_items.pop_front();
    return item;
}

bool Monkey::inspect(unsigned long item, unsigned long lcm, unsigned long& newvalue)
{
    newvalue = this->m_update(item, lcm);
    newvalue /= this->m_worry_divider;
    bool is_divi = newvalue % this->m_divider == 0 ? true : false;

    this->m_numinspect++;

    return is_divi;
}

// End Monkey class

// Embedding round behaviour in class

class MonkeyGroup
{
    private:
        std::vector<Monkey> m_group;
        std::vector<unsigned int> m_ixs_true;
        std::vector<unsigned int> m_ixs_false;
        unsigned long m_divider_prod = 1L;
    public:
        MonkeyGroup(const std::vector<std::string>& lines, const int& worry_div);
        void do_round();
        unsigned long get_monkey_business();
};

MonkeyGroup::MonkeyGroup(const std::vector<std::string>& lines, const int& worry_div)
{
    Monkey cmonkey;
    size_t posheader;
    size_t pos = 0;
    int monkey_inst = 7;
    unsigned char op;
    std::string cline, tmp;

    for(auto im = 0; im < lines.size(); im+=7)
    {
        // Check operation
        cline = lines[im + 2];
        posheader = cline.find("= old") + 6;
        op = cline.substr(posheader, 1)[0];
        tmp = cline.substr(posheader + 2);
        std::function<unsigned long(unsigned long, unsigned long)> fun;
        switch(op)
        {
            case '+':
                if(tmp == "old")
                    fun = [](unsigned long item, unsigned long lcm)
                        {
                            unsigned long val = 2 * item;
                            return lcm == 1 ? val : val % lcm;
                        };
                else
                    fun = [tmp](unsigned long item, unsigned long lcm)
                        {
                            unsigned long val = item + std::stoi(tmp);
                            return lcm == 1 ? val : val % lcm;
                        };
                break;
            case '*':
                if(tmp == "old")
                    fun = [](unsigned long item, unsigned long lcm)
                        {
                            unsigned long val = item * item;
                            return lcm == 1 ? val : val % lcm;
                        };
                else
                    fun = [tmp](unsigned long item, unsigned long lcm)
                        {
                            unsigned long val = item * std::stoi(tmp);
                            return lcm == 1 ? val : val % lcm;
                        };
                break;
            default:
                std::cout << "op not found" << std::endl;
                break;
        }

        // Check test
        cline = lines[im + 3];
        posheader = cline.find("by") + 3;

        // Create Monkey
        cmonkey = Monkey(0, std::stoi(cline.substr(posheader)), worry_div, fun);
        if(worry_div == 1) this->m_divider_prod *= cmonkey.get_divider();

        // Check items
        cline = lines[im + 1];
        posheader = cline.find("items:") + 7;
        pos = 0;
        while(pos < std::string::npos)
        {
            pos = cline.find(",", posheader);
            cmonkey.add_item(std::stoi(cline.substr(posheader, pos - posheader)));
            posheader = pos + 1;
        }

        // Check true
        cline = lines[im + 4];
        posheader = cline.find("monkey") + 7;
        this->m_ixs_true.push_back(std::stoi(cline.substr(posheader)));

        // Check false
        cline = lines[im + 5];
        posheader = cline.find("monkey") + 7;
        this->m_ixs_false.push_back(std::stoi(cline.substr(posheader)));

        this->m_group.push_back(cmonkey);
    }
}

void MonkeyGroup::do_round()
{
    unsigned long item, citem;
    unsigned int cix, move_ix;
    bool is_divi;
    for(auto it = this->m_group.begin(); it != this->m_group.end(); it++)
    {
        cix = std::distance(m_group.begin(), it);

        while(!it->is_empty())
        {
            item = it->get_item();
            is_divi = it->inspect(item, this->m_divider_prod, citem);

            if(is_divi) move_ix = m_ixs_true[cix];
            else move_ix = this->m_ixs_false[cix];

            this->m_group[move_ix].add_item(citem);
        }
    }
}

unsigned long MonkeyGroup::get_monkey_business()
{
    std::vector<unsigned long> bvalues;
    unsigned long business;

    if(m_group.size() > 0)
    {
        std::transform(this->m_group.begin(), this->m_group.end(),
            std::back_inserter(bvalues),
            [](Monkey& mm){return static_cast<unsigned long>(mm.get_num_inspect());});

        std::sort(bvalues.begin(), bvalues.end(), std::greater<unsigned long>());
        business = bvalues[0] * bvalues[1];
    }
    else
        business = 0;

    return business;
}

// End MonkeyGroup class

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line;
    std::vector<std::string> lines;

    while(std::getline(file, line))
        lines.push_back(line);

    int worry_div = 3;
    MonkeyGroup group(lines, worry_div);

    int stop = 20;
    for(auto i = 0; i < stop; i++)
        group.do_round();

    int monkeyval = group.get_monkey_business();
    std::cout << "[" << worry_div << "-Divided] Monkey business after ";
    std::cout << stop << " rounds: " << monkeyval << std::endl;
    return 0;
}

// Comment

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string line;
    std::vector<std::string> lines;

    while(std::getline(file, line))
        lines.push_back(line);

    int worry_div = 1;
    MonkeyGroup group(lines, worry_div);

    int stop = 10000;
    for(auto i = 0; i < stop; i++)
        group.do_round();

    unsigned long monkeyval = group.get_monkey_business();
    std::cout << "[" << worry_div << "-Divided] Monkey business after ";
    std::cout << stop << " rounds: " << monkeyval << std::endl;
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
