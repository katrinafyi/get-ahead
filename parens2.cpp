#include <iostream>
#include <climits>

using namespace std;

int solve(const string& base, const string& target) {
    int table[target.length() + 1][base.length() + 1];

    // base: 0 inserts needed to have a prefix of length 0.
    for (int j = 0; j <= base.length(); j++) {
        table[0][j] = 0;
    }
    // base: i inserts needed to have prefix of length i entirely before base.
    for (int i = 0; i <= target.length(); i++) {
        table[i][0] = i;
    }

    // inserts required to make target[0..i] a prefix of base[j..].
    for (int i = 1; i <= target.length(); i++) {
        for (int j = 1; j <= base.length(); j++) {
            int m = table[i-1][j] + 1;
            if (target[i-1] == base[j-1])
                m = min(m, table[i-1][j-1]);
            table[i][j] = m;
        }
    }

    int m = INT_MAX;
    for (int j = 0; j <= base.length(); j++) {
        m = min(table[target.length()][j], m);
    }
    return m;
}

int main(int argc, char** argv) {
    string base;
    string target;
    if (argc < 3) {
        getline(cin, base);
        getline(cin, target);
    } else {
        base = string(argv[1]);
        target = string(argv[2]);
    }

    cout << "base: " << base << endl;
    cout << "target: " << target << endl;
    cout << "solution: " << solve(base, target) << endl;

    return 0;
}
