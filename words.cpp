#include <iostream>
#include <fstream>

#include <set>
#include <unordered_set>
#include <string>
#include <vector>

constexpr int kPrefixLength = 4;

struct Data {
    std::vector<std::string> grid;
    std::set<std::string> words;
};

struct Coord {
    int row;
    int col;
    
    bool operator==(const Coord& other) const {
        return row == other.row && col == other.col;
    }
};

namespace std {
    template<>
    struct hash<Coord> {
        size_t operator()(const Coord& coord) const {
            return std::hash<int>()(coord.row) * 31 + std::hash<int>()(coord.col);
        }
    };
};

struct Item {
    Coord pos;
    std::string word;
    std::unordered_set<Coord> seen;
};

int LongestWord(const Data& data, const Coord start) {

    const std::vector<std::string>& grid = data.grid;
    const std::set<std::string>& words = data.words;

    int dim = grid.size();

    std::vector<Item> stack;

    auto letter_at = [&grid](Coord coord) {
        return grid[coord.row][coord.col];
    };

    std::vector<int> shifts{ 0, 1, -1 };

    stack.push_back(Item{ start, { letter_at(start) },
            std::unordered_set{ start } });

    std::string longest;
    while (!stack.empty()) {
        Item item = stack.back();
        stack.pop_back();

        std::set<std::string>::const_iterator ceiling = words.lower_bound(item.word);
        if (ceiling == words.end() || ceiling->compare(0, item.word.size(), item.word) != 0) {
            continue;
        }

        if (words.count(item.word) && item.word.size() > longest.size()) {
            longest = item.word;
        }

        // std::cout << item.pos.row << "," << item.pos.col << ": >" << item.word << "<" << std::endl;
        
        // for each of the 8 adjacent positions
        for (int dr : shifts) {
            for (int dc : shifts) {

                if (dr == 0 && dc == 0)
                    continue;

                Coord new_coord = item.pos;
                new_coord.row += dr;
                new_coord.col += dc;

                if (0 <= new_coord.row && new_coord.row < dim &&
                        0 <= new_coord.col && new_coord.col < dim &&
                        !item.seen.count(new_coord)) {

                    std::string suffix{ letter_at(new_coord) };
                    std::unordered_set<Coord> new_seen(item.seen);
                    new_seen.insert(new_coord);

                    stack.push_back(Item{ new_coord, item.word + suffix, new_seen });
                }
            }
        }
    }

    std::cout << "longest: " << longest << std::endl;

    return longest.size();
}

int main(int argc, char** argv) {
    std::string grid_file = "grid.txt";
    if (argc > 2)
        grid_file = argv[1];

    std::string word_file = "words.txt";
    if (argc > 2)
        word_file = argv[2];

    std::set<std::string> words;
    std::unordered_set<std::string> prefixes; // prefixes of length 4

    std::ifstream word_stream(word_file);
    if (!word_stream.is_open()) {
        std::cerr << "failed to open words file" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(word_stream, line)) {
        // uppercase words
        for (char& c : line)
            c = toupper(c);
        // insert word and prefix
        words.insert(line);
        prefixes.insert(line.substr(0, kPrefixLength));
    }
    word_stream.close();

    std::ifstream grid_stream(grid_file);
    if (!grid_stream.is_open()) {
        std::cerr << "failed to open grid file" << std::endl;
        return 1;
    }

    std::vector<std::string> grid;
    while (std::getline(grid_stream, line)) {
        grid.push_back(line);
    }
    grid_stream.close();


    std::cout << words.size() << " words in list" << std::endl;

    Data data = { grid, words };

    int n = grid.size();
    int max = 0;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            max = std::max(max, LongestWord(data, {r, c}));
        }
    }

    std::cout << "solution: " << max << std::endl;
    return 0;
}
