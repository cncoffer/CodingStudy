class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        unordered_map<int, int> map;
        for (auto ele : nums1)
            map[ele]++;

        vector<int> result;
        for (auto ele : nums2)
            if (map[ele]-- > 0)
                result.push_back(ele);
        return result;
    }
};