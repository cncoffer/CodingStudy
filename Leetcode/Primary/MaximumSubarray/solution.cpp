class Solution {
public:
	int maxSubArray(vector<int>& nums) {
		if (nums.empty())
			return INT_MIN;

		int maxsum, lastsum;
		maxsum = nums[0];
		lastsum = max(0, nums[0]);
		for (int i = 1; i < nums.size(); ++i)
		{
			lastsum += nums[i];
			maxsum = max(maxsum, lastsum);
			lastsum = max(0, lastsum);
		}

		return maxsum;
	}
};