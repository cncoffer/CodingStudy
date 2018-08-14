class Solution {
public:
    void sortColors(vector<int>& nums) {
        int a[3] = {0};
        for (auto ele : nums)
            ++a[ele];
        
        for (auto &ele : nums)
        {
            if (a[0] > 0) {
                ele = 0;
                a[0]--;
            }
            else if (a[1] > 0) {
                ele = 1;
                a[1]--;
            }
            else {
                ele = 2;
            }
        }
    }
};

// solution2
class Solution {
public:
	void sortColors(vector<int>& nums) {
		if (nums.empty()) return;
		int left, right;
		left = 0; right = nums.size() - 1;
		while (nums[left] == 0) ++left;
		while (nums[right] == 2) --right;
		int i = left + 1;
		while (i < right) {
			if (nums[left] > nums[i]) {
				swap(nums[left], nums[i]);
				while (nums[left] == 0) ++left;
				i = left + 1;
			}
			else if (nums[i] > nums[right]) {
				swap(nums[right], nums[i]);
				while (nums[right] == 2) --right;
			}
			else
				i++;
		}
		if (left < right && nums[left] > nums[right])
			swap(nums[left], nums[right]);
	}
};

// solution3
class Solution {
public:
    void sortColors(vector<int>& nums) {
        int size = nums.size();
        if (size <= 1) return;
        int left=0, right=size-1, cur=0;
        while (cur <= right) {
            if (nums[cur] == 0)
                swap(nums[left++], nums[cur++]);
            else if (nums[cur] == 2)
                swap(nums[right--], nums[cur]);
            else
                cur++;
        }
    }
};