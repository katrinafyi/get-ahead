#include <iostream>
#include <fstream>

#include <set>
#include <unordered_set>
#include <string>
#include <vector>

// data passed to function
struct Data {
    std::vector<std::string> grid;
    std::set<std::string> words;
};

// coordinate in grid
struct Coord {
    int row;
    int col;
    
    bool operator==(const Coord& other) const {
        return row == other.row && col == other.col;
    }
};

// hash function for Coord
namespace std {
    template<>
    struct hash<Coord> {
        size_t operator()(const Coord& coord) const {
            return std::hash<int>()(coord.row) * 31 + std::hash<int>()(coord.col);
        }
    };
};

// temporary item used while epxloring grid
struct Item {
    Coord pos;
    std::string word;
    std::unordered_set<Coord> seen;
};

int LongestWord(const Data& data, const Coord start) {
    // shortcut references
    const std::vector<std::string>& grid = data.grid;
    const std::set<std::string>& words = data.words;

    // grid dimensions
    int dim = grid.size();

    // dfs using stack
    std::vector<Item> stack;

    // letter at given coordinate in grid
    auto letter_at = [&grid](Coord coord) {
        return grid[coord.row][coord.col];
    };

    // coordinate shifts
    std::vector<int> shifts{ 0, 1, -1 };

    // start at start position
    stack.push_back(Item{ start, { letter_at(start) },
            std::unordered_set{ start } });

    std::string longest;
    while (!stack.empty()) {
        // pop from top of stack
        Item item = stack.back();
        stack.pop_back();

        // get next word >= the current word part (item.word), called the ceiling.
        // ensure item.word is a prefix of the ceiling word, otherwise terminate this path.
        std::set<std::string>::const_iterator ceiling = words.lower_bound(item.word);
        if (ceiling == words.end() || ceiling->compare(0, item.word.size(), item.word) != 0) {
            continue;
        }

        // if ceiling word and current word part have equal length, current word
        // part appears in the word list.
        // keep track of longest word found which appears in word list.
        if (ceiling->size() == item.word.size() && item.word.size() > longest.size()) {
            longest = item.word;
        }

        // std::cout << item.pos.row << "," << item.pos.col << ": >" << item.word << "<" << std::endl;
        
        // for each of the 8 adjacent positions (excluding dr == dc == 0).
        for (int dr : shifts) {
            for (int dc : shifts) {
                if (dr == 0 && dc == 0)
                    continue;

                // coordinate of adjacent position
                Coord new_coord = item.pos;
                new_coord.row += dr;
                new_coord.col += dc;

                // ensure new coordinate is in bounds and has not been visited
                if (0 <= new_coord.row && new_coord.row < dim &&
                        0 <= new_coord.col && new_coord.col < dim &&
                        !item.seen.count(new_coord)) {

                    // create new string by appending next character
                    std::string new_word = item.word;
                    new_word += letter_at(new_coord);
                    // create new seen set by inserting new coordinate
                    std::unordered_set<Coord> new_seen(item.seen);
                    new_seen.insert(new_coord);

                    stack.push_back(Item{ new_coord, new_word, new_seen });
                }
            }
        }
    }

    std::cout << "longest: " << longest << std::endl;

    return longest.size();
}

int main(int argc, char** argv) {
    std::string grid_file = "grid.txt";
    if (argc >= 2)
        grid_file = argv[1];

    std::string word_file = "words.txt";
    if (argc >= 3)
        word_file = argv[2];

    std::set<std::string> words;

    std::ifstream word_stream(word_file);
    if (!word_stream.is_open()) {
        std::cerr << "failed to open words file" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(word_stream, line)) {
        // uppercase then insert to set
        for (char& c : line)
            c = toupper(c);
        words.insert(line);
    }
    word_stream.close();
    std::cout << words.size() << " words in list" << std::endl;


    std::ifstream grid_stream(grid_file);
    if (!grid_stream.is_open()) {
        std::cerr << "failed to open grid file" << std::endl;
        return 1;
    }

    std::cout << "grid:" << std::endl;
    std::vector<std::string> grid;
    while (std::getline(grid_stream, line)) {
        std::cout << line << std::endl;
        grid.push_back(line);
    }
    grid_stream.close();


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
