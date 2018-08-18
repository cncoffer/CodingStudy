class Solution {
public:
    int titleToNumber(string s) {
        if (s.empty()) return 0;
        int result = 0;
        for (auto c : s) {
            result *= 26;
            result += c - 'A' + 1;
        }
        return result;
    }
};