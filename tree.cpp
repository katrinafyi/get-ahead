#include <iostream>
#include <vector>
#include <string_view>

struct TreeNode {
    int value;
    std::vector<TreeNode> children;
};

// intermediate result struct.
struct Result {
    // length of longest path.
    int length;
    // true if the longest path reaches the root node.
    bool top;
};

#define TOP_PREFIX "+-"
#define MID_PREFIX "| "
#define BOT_PREFIX "  "
#define INDENT 2

void print_tree(const TreeNode& node, const std::string& prefix) {
    if (prefix.size() > 0) {
        // print prefix but replace last character with +
        std::string_view prefixView{ prefix };
        std::cout << prefixView.substr(0, prefix.size() - INDENT) << TOP_PREFIX;
    }

    // print value of this node
    std::cout << node.value << std::endl;
    
    // prefixes for recursive calls, depending on whether there are remaining
    // children at this level
    std::string midPrefix{ prefix + MID_PREFIX };
    std::string endPrefix{ prefix + BOT_PREFIX };

    // recursively print, using appropriate prefix
    int i = 0;
    int n = node.children.size();
    for (const TreeNode& child : node.children) {
        print_tree(child, i+1 < n ? midPrefix : endPrefix);
        i++;
    }
}

Result solve(const TreeNode& node) {
    Result result = {0, true};

    for (const TreeNode& child : node.children) {
        Result childResult = solve(child);
    
        // 2 options:
        // - longest path is longest path of child, or
        // - longest path is longest path of child + 1 if this node and child
        //   are consecutive and child's longest path reaches the top.

        int m = childResult.length;
        bool top = false;

        if (child.value == node.value + 1 && childResult.top) {
            m = childResult.length + 1;
            top = true;
        }

        // prioritise results with longer length OR top flag set.
        if (m >= result.length && (m > result.length || top)) {
            result.length = m;
            result.top = top;
        }
    }

    // std::cout << node.value << ": " << result.length << ',' << result.top << std::endl;
    return result;
}

int main(int argc, char** argv) {
    std::vector<TreeNode> tests = {
        {1, {{2, {{4}}}, {3}}},
        {5, {{12}, {7, {{8, {{9, {{15}, {10}}}}}, {6}}}}},
        {1, {{2, {{10, {{11, {{12, {{13, {{14}}}}}}}}}}}}},
    };

    for (const TreeNode& tree : tests) {
        print_tree(tree, "");
        Result result = solve(tree);
        std::cout << "solution: " << 1 + result.length << std::endl << std::endl;
    }

    return 0;
}
