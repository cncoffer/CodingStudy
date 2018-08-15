class Solution {
public:
	int search(vector<int>& nums, int target) {
		return recSearch(nums, 0, nums.size() - 1, target);
	}
private:
	int binarySearch(vector<int> &nums, int left, int right, int target) {
		while (left <= right) {
			int mid = static_cast<int>(((long)left + (long)right) / 2);
			if (nums[mid] == target) return mid;
			else if (nums[mid] > target) right = mid - 1;
			else left = mid + 1;
		}
		return -1;
	}
	int recSearch(vector<int> &nums, int left, int right, int target) {
		if (left > right) return -1;
		int mid = static_cast<int>(((long)left + (long)right) / 2);
		if (nums[mid] == target) return mid;
		if (nums[left] < nums[mid]) {
			if (nums[left] <= target && target < nums[mid])
				return binarySearch(nums, left, mid - 1, target);
			else
				return recSearch(nums, mid + 1, right, target);
		}
		else {
			if (nums[mid+1] <= target && target <= nums[right])
				return binarySearch(nums, mid + 1, right, target);
			else
				return recSearch(nums, left, mid - 1, target);
		}
	}
};