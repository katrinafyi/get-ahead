#include <iostream>
#include <vector>
#include <string_view>

struct TreeNode {
    int value;
    std::vector<TreeNode> children;
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

int solve(const TreeNode& node) {
    int m = 0;
    for (const TreeNode& child : node.children) {
        m = std::max(m,
                (child.value == node.value + 1 ? 1 : 0)
                + solve(child));
    }
    return m;
}

int main(int argc, char** argv) {
    std::vector<TreeNode> tests = {
        {1, {{2, {{4}}}, {3}}},
        {5, {{12}, {7, {{8, {{9, {{15}, {10}}}}}, {6}}}}}
    };

    for (const TreeNode& tree : tests) {
        print_tree(tree, "");
        std::cout << "solution: " << 1 + solve(tree) << std::endl << std::endl;
    }

    return 0;
}
