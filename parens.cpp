#include <iostream>

using namespace std;


int solve(const string& base, const string& target) {
    int table[1 + target.length()];

    table[0] = 0;
    for (int i = 0; i < target.length(); i++) {
        string targetPart = target.substr(0, i + 1);
        table[i + 1] = table[i] + (base.find(targetPart) != string::npos ? 0 : 1);
    }
    return table[target.length()];
}

int main() {
    string base;
    string target;
    getline(cin, base);
    getline(cin, target);

    cout << "base: " << base << endl;
    cout << "target: " << target << endl;
    cout << "solution: " << solve(base, target) << endl;

    return 0;
}
