class Solution {
public:
	vector<string> generateParenthesis(int n) {
		result.clear();
		if (!n) return result;
		recGenerateParenthesis(n, 0, "");
		return result;
	}
private:
	vector<string> result;
	void recGenerateParenthesis(int n, int right, string tmpstr) {
		if (!n && !right) {
			result.push_back(tmpstr);
			return;
		}
		if (n) {
			tmpstr += '(';
			recGenerateParenthesis(n - 1, right + 1, tmpstr);
			tmpstr.erase(tmpstr.end() - 1);
		}
		if (right) {
			tmpstr += ')';
			recGenerateParenthesis(n, right - 1, tmpstr);
		}
	}
};