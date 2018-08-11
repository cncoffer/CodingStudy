class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.empty())
            return 0;
        
        int lastrob, last_lastrob;
        lastrob = nums[0];
        last_lastrob = 0;
        for (int i = 1; i < nums.size(); ++i)
        {
            int cur = nums[i] + last_lastrob;
            last_lastrob = lastrob;
            lastrob = max(lastrob, cur);
        }
        
        return lastrob;
    }
};