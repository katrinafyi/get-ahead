#include <iostream>
#include <fstream>

#include <unordered_set>
#include <string>
#include <vector>

struct Coord {
public:
    int row;
    int col;
    
    bool operator==(const Coord& other) const {
        return row == other.row && col == other.col;
    }
};

struct Item {
    Coord pos;
    std::string word;
};

int LongestWord(std::vector<std::string> grid, Coord start) {
    int dim = grid.size();

    auto coord_hash = [](const Coord& coord) {
        return std::hash<int>()(coord.row) * 31 + std::hash<int>()(coord.col);
    };

    std::vector<Item> stack;
    std::unordered_set<Coord, decltype(coord_hash)> seen(5, coord_hash);

    auto letter_at = [&grid](Coord coord) {
        return grid[coord.row][coord.col];
    };

    std::vector<Coord> shifts{ {0, 1}, {1, 0}, {-1, 0}, {0, -1} };

    stack.push_back(Item{ start, { letter_at(start) } });

    int longest = 0;
    while (!stack.empty()) {
        Item item = stack.back();
        stack.pop_back();

        if (!seen.insert(item.pos).second) {
            continue; // if seen already has coord, ignore.
        }

        std::cout << item.pos.row << "," << item.pos.col << ": >" << item.word << "<" << std::endl;
        
        for (const Coord& shift : shifts) {
            Coord new_coord = item.pos;
            new_coord.row += shift.row;
            new_coord.col += shift.col;

            if (0 <= new_coord.row && new_coord.row < dim &&
                    0 <= new_coord.col && new_coord.col < dim) {
                std::string suffix{ letter_at(new_coord) };
                stack.push_back(Item{ new_coord, item.word + suffix });
            }
        }
    }

}

int main(int argc, char** argv) {
    std::string grid_file = "grid.txt";
    if (argc > 2)
        grid_file = argv[1];

    std::string word_file = "words.txt";
    if (argc > 2)
        word_file = argv[2];

    std::unordered_set<std::string> words;

    std::ifstream word_stream(word_file);
    if (!word_stream.is_open()) {
        std::cerr << "failed to open words file" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(word_stream, line)) {
        words.insert(line);
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

    int n = grid.size();
    int max = 0;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            max = std::max(max, LongestWord(grid, {r, c}));
            break;
        }
    }

    std::cout << "solution: " << max << std::endl;
    return 0;
}
