class Solution {
public:
	int numIslands(vector<vector<char>>& grid) {
		int islands = 0;
		for (int i = 0; i < grid.size(); ++i)
			for (int j = 0; j < grid[i].size(); ++j) {
				if (grid[i][j] == '1') {
					++islands;
					recClearIsland(grid, i, j);
				}
			}
		return islands;
	}
private:
	void recClearIsland(vector<vector<char>>& grid, int i, int j) {
		if (grid[i][j] == '0') return;
		grid[i][j] = '0';
		if (i - 1 >= 0) recClearIsland(grid, i - 1, j);
		if (i + 1 < grid.size()) recClearIsland(grid, i + 1, j);
		if (j - 1 >= 0) recClearIsland(grid, i, j - 1);
		if (j + 1 < grid[i].size()) recClearIsland(grid, i, j + 1);
	}
};