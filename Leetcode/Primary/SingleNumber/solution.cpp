class Solution {
public:
    int singleNumber(vector<int>& nums) {
        unordered_map<int, int> map;
        for (auto ele : nums)
        {
            map[ele]++;
        }

        for (auto ele : map)
        {
            if (ele.second == 1)
                return ele.first;
        }

        return 0;
    }
};