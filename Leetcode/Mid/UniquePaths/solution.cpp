class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>> result(m);
        vector<int> tmp(n);
        fill(tmp.begin(), tmp.end(), 0);
        fill(result.begin(), result.end(), tmp);

        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j) {
                if (i == 0 || j == 0)
                    result[i][j] = 1;
                else
                    result[i][j] = result[i][j-1] + result[i-1][j];
            }
        
        return result[m-1][n-1];
    }
};