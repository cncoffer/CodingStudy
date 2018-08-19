class Solution {
public:
	vector<int> spiralOrder(vector<vector<int>>& matrix) {
		vector<int> result;
		if (matrix.empty() || matrix[0].empty()) return result;
		vector<bool> tmp(matrix[0].size(), false);
		vector<vector<bool>> isVisited(matrix.size(), tmp);
		isVisited[0][0] = true;
		result.push_back(matrix[0][0]);
		int row = 0, col = 0;
		Direct direct = Direct::right;
		while (getNextIndex(row, col, direct, isVisited) != Direct::none) {
			result.push_back(matrix[row][col]);
			isVisited[row][col] = true;
		}
		return result;
	}
private:
	enum Direct {
		none = 0,
		right = 1,
		bottom = 2,
		left = 3,
		top = 4
	};
	bool getNextIndex(int &row, int &col, Direct &direct, vector<vector<bool>> &isVisited) {
		for (int i = 0; i < 4; ++i) { // try 4 direct
			switch (direct)
			{
			case Direct::right:
				if (col + 1 < isVisited[0].size() && !isVisited[row][col + 1]) {
					++col;
					return true;
				}
				else
					direct = Direct::bottom;
				break;
			case Direct::bottom:
				if (row + 1 < isVisited.size() && !isVisited[row + 1][col]) {
					++row;
					return true;
				}
				else
					direct = Direct::left;
				break;
			case Direct::left:
				if (col - 1 >= 0 && !isVisited[row][col - 1]) {
					--col;
					return true;
				}
				else
					direct = Direct::top;
				break;
			case Direct::top:
				if (row - 1 >= 0 && !isVisited[row - 1][col]) {
					--row;
					return true;
				}
				else
					direct = Direct::right;
				break;
			default:
				break;
			}
		}

		return false;
	}
};