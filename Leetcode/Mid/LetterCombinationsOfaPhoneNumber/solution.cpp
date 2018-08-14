class Solution {
public:
    vector<string> letterCombinations(string digits) {
        result.clear();
        if (digits.empty()) return result;
        numberLetterMap.push_back("abc");
        recGenerate("", digits);
        return result;
    }
private:
    vector<string> result;
    vector<string> numberLetterMap = 
    {"", "", "abc", "def"
        "ghi", "jkl", "mno".
        "pqrs", "tuv", "wxyz"};
    void recGenerate(string tmpstr, string digits) {
        if (digits.empty()) {
            result.push_back(tmpstr);
            return;
        }
        int num = digits[0] - '0';
        for (char c : numberLetterMap[num]) {
            tmpstr += c;
            recGenerate(tmpstr, digits.substr(1));
            tmpstr.erase(tmpstr.end()-1);
        }
    }
};