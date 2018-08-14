class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        vector<int> result;
        if (!k) return result;

        unordered_map<int, int> nmap;
        for (int num : nums)
            nmap[num]++;
	    typedef pair<int, int> intpair;
        vector<intpair> sortnums;
        for (intpair numpair : nmap)
            sortnums.push_back(numpair);
        sort(sortnums.begin(), sortnums.end(), [](intpair a, intpair b) { return a.second > b.second;});
        for (int i = 0; i < k; ++i)
            result.push_back(sortnums[i].first);
        return result;
    }
};

// solution3
class Solution {
public:
	vector<int> topKFrequent(vector<int>& nums, int k) {
		unordered_map<int, int> m;
		vector<vector<int>> bucket(nums.size() + 1);
		vector<int> res;
		for (auto a : nums) ++m[a];
		for (auto it : m) {
			bucket[it.second].push_back(it.first);
		}
		for (int i = nums.size(); i >= 0; --i) {
			for (int j = 0; j < bucket[i].size(); ++j) {
				res.push_back(bucket[i][j]);
				if (res.size() == k) return res;
			}
		}
		return res;
	}
};