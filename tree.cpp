#include <iostream>
#include <vector>
#include <string_view>

struct TreeNode {
    int value;
    std::vector<TreeNode> children;
};

// intermediate result struct.
struct Result {
    // length of longest path possibly not reaching root.
    int longest;
    // length of longest path reaching root.
    int top;
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
    Result result = {0, 0};

    for (const TreeNode& child : node.children) {
        Result childResult = solve(child);

        // longest path could be longest path of child.
        result.longest = std::max(result.longest, childResult.longest);

        // if consecutive, longest path reaching top is longest top path of
        // child + 1.
        if (child.value == node.value + 1) {
            result.top = std::max(result.top, 1 + childResult.top);
        }
    }

    // longest path could also be longest path reaching top.
    result.longest = std::max(result.longest, result.top);

    // std::cout << node.value << ": " << result.length << ',' << result.top << std::endl;
    return result;
}

#define ONE(val, child) {val, {child}}
#define TWO(val, left, right) {val, {left, right}}

int main(int argc, char** argv) {
    std::vector<TreeNode> tests = {
        {1, {{2, {{4}}}, {3}}},
        {5, {{12}, {7, {{8, {{9, {{15}, {10}}}}}, {6}}}}},
        {1, {{2, {{10, {{11, {{12, {{13, {{14}}}}}}}}}}}}},
        ONE(8, ONE(9, TWO(10, 
                        ONE(1, ONE(2, ONE(3, {4}))),
                        ONE(11, ONE(12, {13}))))),
    };

    for (const TreeNode& tree : tests) {
        print_tree(tree, "");
        Result result = solve(tree);
        std::cout << "solution: " << 1 + result.longest << std::endl << std::endl;
    }

    return 0;
}
