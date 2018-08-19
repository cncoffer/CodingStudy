class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        if (nums.empty()) return 1;
        sort(nums.begin(), nums.end());
        int missingNum = 1;
        for (int num : nums) {
            if (num == missingNum)
                ++missingNum;
            else if (num > missingNum)
                break;
        }
        return missingNum;
    }
};

// solution2
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        unordered_set<int> numset;
        for (int num : nums)
            numset.insert(num);
        int i;
        for (i = 1; i < numset.size()+1; ++i) {
            if (numset.find(i) == numset.end())
                break;
        }
        return i;
    }
};

// solution3
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        for (int i = 0; i < nums.size(); i++) {
            int cur = i;
            while (nums[cur] > 0 && nums[cur] <= nums.size() &&
                  nums[cur] != cur + 1 && nums[nums[cur] - 1] != nums[cur]) {
                swap(nums[nums[cur] - 1], nums[cur]);
            }
        }
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] != i + 1) {
                return i + 1;
            }
        }
        return nums.size() + 1;
    }
};

// solution3 modify
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        int i = 0;
        while (i < nums.size()) {
            int cur = i;
            if (nums[i] > 0 && nums[i] <= nums.size() && nums[cur] != cur + 1 && nums[nums[cur] - 1] != nums[cur]) {
                swap(nums[nums[cur] - 1], nums[cur]);
                continue;
            }
            ++i;
        }
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] != i + 1)
                return i + 1;
        }
        return nums.size() + 1;
    }
};