// solution 2
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        if (nums.empty()) return false;

        unordered_map<int, int> nummap;
        for (auto ele : nums) {
            ++nummap[ele];
        }

        for (auto pairele : nummap) {
            if (pairele.second > 1)
                return true;
        }
        return false;
    }
};

// solution 2.5
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        if (nums.empty()) return false;

        unordered_set<int> numset;
        for (auto ele : nums) {
            if (numset.find(ele) != numset.end()) return true;
            numset.insert(ele);
        }
        return false;
    }
};

