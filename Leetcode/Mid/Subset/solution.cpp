class Solution {
public:
	vector<vector<int>> subsets(vector<int>& nums) {
		result.clear();
		for (int i = 0; i <= nums.size(); ++i) {
			vector<int> tmp;
			recGenerateSubsets(nums, i, 0, tmp);
		}
		return result;
	}
private:
	vector<vector<int>> result;
	void recGenerateSubsets(vector<int>& nums, int remainder, int left, vector<int>& tmpRet) {
		if (remainder <= 0) {
			result.push_back(tmpRet);
			return;
		}
		else if (left >= nums.size() || remainder > nums.size())
			return;
		else if (left + remainder == nums.size()) {
			vector<int> tmp(tmpRet);
			for (int i = left; i < nums.size(); ++i)
				tmp.push_back(nums[i]);
			result.push_back(tmp);
			return;
		}
		for (int i = left; i < nums.size(); ++i) {
			tmpRet.push_back(nums[i]);
			recGenerateSubsets(nums, remainder - 1, i + 1, tmpRet);
			tmpRet.pop_back();
		}
	}
};