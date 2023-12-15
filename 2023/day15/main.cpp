#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <array>
#include <vector>
#include <utility>
#include <algorithm>

std::string HEADER_DELIM = "--------";

int help()
{
    std::cout << "Day 15 2023" << std::endl;
    std::cout << "Lens library" << std::endl;
    std::cout << "Usage: day15_main input_file" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "input_file   INPUT: file to parse to solve the puzzle." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "(Other arguments are ignored.)" << std::endl;

    return 1;
}

// Process word into hash
size_t hash_word(const std::string& word){
    size_t val = 0;
    for(const auto& ll : word){
        if(ll == '\n')
            continue;
        val += static_cast<size_t>(ll);
        val *= 17;
        val %= 256;
    }

    return val;
}

int main_puzzle1(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string word;
    size_t checksum = 0;
    while(std::getline(file, word, ','))
        checksum += hash_word(word);

    std::cout << "Sum of checksum: " << checksum << std::endl;
    return 0;
}

// Process word and find: label, lens value, box id, operation: add or remove
void find_label_operation(
    const std::string& word,
    std::string& label,
    size_t& lens,
    size_t& id_box,
    bool& to_add
){
    size_t npos = word.find('=');
    to_add = npos != std::string::npos;
    if((!to_add) && (word.back() != '-'))
        throw std::runtime_error("Invalid word: no operation found.");
    else if(to_add)
        lens = std::stoi(word.substr(npos + 1));
    else{ // to remove
        npos = word.length() - 1;
        lens = 0; // not useful
    }

    label = word.substr(0, npos);
    id_box = hash_word(label);
}

// Alias
typedef std::vector<std::pair<std::string, size_t>> LensBox;

// Update boxes container from word
void update_boxes(
    const std::string& word,
    std::array<LensBox, 256>& boxes
){
    std::string label;
    size_t id_box = 0, lens = 0;
    bool to_add;
    find_label_operation(word, label, lens, id_box, to_add);

    if(id_box > 256)
        throw std::runtime_error("Invalid box id: " + std::to_string(id_box));

    auto it = std::find_if(
        boxes[id_box].begin(),
        boxes[id_box].end(),
        [&label](const auto& pair){return pair.first == label;}
    );

    if(to_add){
        if(it != boxes[id_box].end()) // replace lens
            it->second = lens;
        else // insert slot
            boxes[id_box].push_back(std::make_pair(label, lens));
    }
    else{
        if(it != boxes[id_box].end())
            boxes[id_box].erase(it);
        // else: cannot remove someting not added
    }
}

size_t focusing_power(
    const LensBox& box,
    const size_t box_factor
){
    size_t power = 0, ix_slot = 1;
    for(auto it = box.begin(); it != box.end(); it++, ix_slot++)
        power += box_factor * ix_slot * it->second;

    return power;
}

int main_puzzle2(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string word;
    size_t power = 0;
    std::array<LensBox, 256> boxes;
    while(std::getline(file, word, ',')){
        if(word.back() == '\n')
            word = word.substr(0, word.length() - 1);
        update_boxes(word, boxes);
    }

    size_t box_factor = 1; // box factor is box_id + 1
    for(auto it = boxes.begin(); it != boxes.end(); it++, box_factor++){
        if(it->empty())
            continue;
        power += focusing_power(*it, box_factor);
    }

    std::cout << "Sum of focusing power: " << power << std::endl;
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
