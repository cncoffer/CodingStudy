class Solution {
public:
    string longestPalindrome(string s) {
        int length = s.size();
        if (length <= 1) return s;

        int left;
        int maxLength = 0;
        for (int i = 0; i < length; ++i) {
            for (int j = i; j < length; ++j) {
                if (isPalindrome(s, i, j)) {
                    int l = j - i + 1;
                    if (l > maxLength) {
                        maxLength = l;
                        left = i;
                    }
                }
            }
        }

        return s.substr(left, maxLength);
    }
private:
    bool isPalindrome(string s, int left, int right)
    {
        while (left < right)
            if (s[left++] != s[right--]) return false;
        return true;
    }
};

// solution2
class Solution2 {
public:
	string longestPalindrome(string s) {
		int length = s.size();
		if (length <= 1) return s;

		vector<bool> temp(length);
		vector<vector<bool>> arr(length);
		fill(arr.begin(), arr.end(), temp);

		int maxLength = 1, left = 0;
		for (int l = 1; l <= length; ++l) {
			int i = 0;
			int j = i + l - 1;
			do
			{
				if (l == 1) {
					arr[i][i] = true;
				}
				else if (l == 2) {
					if (s[i] == s[j]) {
						maxLength = l;
						left = i;
						arr[i][j] = true;
					}
				}
				else if (s[i] == s[j]) {
					if (arr[i][j] = arr[i + 1][j - 1]) {
						maxLength = l;
						left = i;
					}
				}
				i++;
				j = i + l - 1;
			} while (j < length);
		}
		return s.substr(left, maxLength);
	}
};