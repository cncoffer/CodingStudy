class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        int carry = 1;
        for (int i = digits.size() - 1; i >= 0; --i) {
            digits[i] += carry;
            carry = digits[i] / 10;
            if (carry == 0) break;
            digits[i] %= 10;
        }
        vector<int> ret;
        if (carry > 0) ret.push_back(carry);
        for (auto ele : digits)
            ret.push_back(ele);
        
        return ret;
    }
};