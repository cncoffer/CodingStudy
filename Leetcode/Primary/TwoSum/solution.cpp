class Solution {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		vector<int> ret;
		int size = nums.size();
		if (size < 2)
			return ret;

		unordered_map<int, int> map;
		for (int i = 0; i < size; i++)
			map[nums[i]] = i;

		for (int i = 0; i < size; i++)
		{
			int find_num = target - nums[i];
			if (map.find(find_num) != map.end() && map[find_num] != i)
			{
				ret.push_back(i);
				ret.push_back(map[find_num]);
				break;
			}
		}

		return ret;
	}
};

class Solution2 {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        std::unordered_map<int, vector<int>::size_type> m;
        for (vector<int>::size_type i = 0; i != nums.size(); ++i) {
            if (m.find(nums[i]) != m.end()) {
                vector<int> res({m[nums[i]]}, i);
                return res;
            } else {
                m[target - nums[i]] = i;
            }
        }
        return {};
    }
};