#include <iostream>
#include <vector>
#include <climits>

template<class T>
using Vec = std::vector<T>;

struct Result {
    // sum of values in rectangle.
    int sum;
    // row index of top left corner in rectangle.
    int top;
    // row index of bottom right corner in rectangle.
    int bottom;
    // height of rectangle (rightwards in diagram).
    int height;
};

void print_matrix(const Vec<Vec<int>>& matrix) {
    for (const Vec<int>& row : matrix) {
        for (const int& x : row) {
            std::cout << x << ' ';
        }
        std::cout << std::endl;
    }
}

Result solve(Vec<Vec<int>>& data) {
    int rows = data.size();

    // heights[i][j] contains the maximum heigth of the rectangle from row i to j.
    int heights[rows][rows] = {0};

    for (int i = 0; i < rows; i++) {
        heights[i][i] = data[i].size();
    }
    
    // fill in heights matrix diagonally.
    for (int i = 1; i < rows; i++) {
        int r = 0;
        int c = i;
        while (c < rows) {
            heights[r][c] = std::min(heights[r][c-1], heights[r+1][c]);
            c++;
            r++;
        }
    }

    std::cout << "heights:" << std::endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            std::cout << heights[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    Vec<Vec<int>> prefixes;
    for (Vec<int> row : data) {
        Vec<int> rowPrefix{ 0 };
        for (int x : row) {
            rowPrefix.push_back(rowPrefix.back() + x);
        }
        prefixes.push_back(rowPrefix);
    }

    std::cout << "prefixes:" << std::endl;
    print_matrix(prefixes);
    
    Result result{ 0 };
    for (int i = 0; i < rows; i++) {
        for (int j = i; j < rows; j++) {
            int h = heights[i][j];
            int sum = 0;
            for (int k = i; k <= j; k++) {
                sum += prefixes[k][h];
            }
            if (sum > result.sum) {
                result.sum = sum;
                result.top = i;
                result.bottom = j;
                result.height = h;
            }
        }
    }

    return result;
}

void print_result(const Result& result) {
    std::cout << "sum: " << result.sum << std::endl;
    // too hard to print with cout
    printf("(%d, %d) to (%d, %d)\n",
            result.top, 0,
            result.bottom + 1, result.height);
}

int main() {
    Vec<Vec<Vec<int>>> tests = {
        {{1, 2}, {3, 4, 5, 6}, {7, 8}, {9}},
        {{1, 3, 2, 2}, {2, 1, 2, 3}, {4, 2, 3}, {1, 1, 2, 17, 14}, {3, 1, 2, 2}},
    };
    int i = 0;
    for (Vec<Vec<int>>& test : tests) {
        std::cout << "test " << i << std::endl;
        // print_matrix(test);
        print_result(solve(test));
        std::cout << std::endl;
        i++;
    }
}
