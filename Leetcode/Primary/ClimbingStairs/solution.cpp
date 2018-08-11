class Solution {
public:
    int climbStairs(int n) {
        vector<int> vt(n+1);
        vt[1] = 1;
        vt[2] = 2;
        if (n <= 2)
            return vt[n];

        for (int i = 3; i <= n; ++i)
            vt[i] = vt[i-1] + vt[i-2];

        return vt[n];        
    }
};