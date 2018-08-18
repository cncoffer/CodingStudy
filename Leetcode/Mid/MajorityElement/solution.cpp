class Solution {
public:
    int majorityElement(vector<int>& nums) {
        unordered_map<int, int> m;
        int halfsize = (nums.size()-1) / 2;
        int ret;
        for (auto elem : nums) {
            ++m[elem];
            if (m[elem] > halfsize) {
                ret = elem;
                break;
            }
        }
        return ret;
    }
};

// solution3
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int res = 0, cnt = 0;
        for (int num : nums) {
            if (cnt == 0) {res = num; ++cnt;}
            else (num == res) ? ++cnt : --cnt;
        }
        return res;
    }
};