class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        result.clear();
        if (nums.empty()) return result;
        vector<int> tmp;
        recGeneratePermute(nums, tmp);
        return result;
    }
private:
    vector<vector<int>> result;
    void recGeneratePermute(vector<int>& nums, vector<int>& tmp) {
        if (nums.empty()) {
            result.push_back(tmp);
            return;
        }
        int size = nums.size();
        for (int i = 0; i < size; ++i) {
            int num = nums[i];
            nums.erase(nums.begin() + i);
            tmp.push_back(num);
            recGeneratePermute(nums, tmp);
            nums.insert(nums.begin() + i, num);
            tmp.pop_back();
        }
    }
};