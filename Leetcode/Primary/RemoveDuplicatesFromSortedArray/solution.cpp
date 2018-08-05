class Solution {
public:
	int removeDuplicates(vector<int>& nums) {
		if (!nums.size())
			return 0;
            
		vector<int>::iterator iter_slow;
		iter_slow = nums.begin();

		// traverse from scound element
		for (auto iter_fast = nums.begin() + 1; iter_fast != nums.end(); ++iter_fast)
		{
			if (*iter_fast != *iter_slow)
				*(++iter_slow) = *iter_fast;
		}

		return iter_slow - nums.begin() + 1;
	}
};