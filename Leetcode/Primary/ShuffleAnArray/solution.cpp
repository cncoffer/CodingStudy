class Solution {
public:
    Solution(vector<int> nums) {
        ori = nums;
    }
    
    /** Resets the array to its original configuration and return it. */
    vector<int> reset() {
        return ori;
    }
    
    /** Returns a random shuffling of the array. */
    vector<int> shuffle() {
        vector<int> ret = ori;
        random_shuffle(ret.begin(), ret.end());
        return ret;
    }
private:
    vector<int> ori;
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution obj = new Solution(nums);
 * vector<int> param_1 = obj.reset();
 * vector<int> param_2 = obj.shuffle();
 */

// solution2
class Solution {
public:
    Solution(vector<int> nums) {
        ori = nums;
        permutation(result, nums, 0);
    }
    
    /** Resets the array to its original configuration and return it. */
    vector<int> reset() {
        return ori;
    }
    
    /** Returns a random shuffling of the array. */
    vector<int> shuffle() {
	    uniform_int_distribution<int> di(0, result.size()-1);
        return result[di(dre)];
    }
private:
    vector<int> ori;
    vector<vector<int>> result;
	default_random_engine dre;

    void permutation(vector<vector<int>>& res, vector<int>& num, int index) {
        if (index >= num.size()) {
            res.push_back(num);
            return;
        }
        for (int i = index; i < num.size(); i++) {
            swap(num[i], num[index]);
            permutation(res, num, index + 1);
            swap(num[index], num[i]);
        }
    }
};

// solution3
class Solution {
public:
    Solution(vector<int> nums):vec(nums) {
        
    }
    
    /** Resets the array to its original configuration and return it. */
    vector<int> reset() {
        return vec;
    }
    
    /** Returns a random shuffling of the array. */
    vector<int> shuffle() {
        if(vec.size()==0) return {};
        vector<int> tem(vec);
        int len = vec.size();
        for(int i = 0; i < len; i++)
        {
            int pos = rand()%(len-i);
            swap(tem[i+pos], tem[i]);
        }
        return tem;
    }
private:
    vector<int> vec;
};
