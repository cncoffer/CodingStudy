class Solution {
public:
#define LetterNum 128
	int lengthOfLongestSubstring(string s) {
		int length = s.size();
		if (length <= 1) return length;

		int left = 0, right = 0, maxLength = 0;
		vector<int> letterIndex(LetterNum);
		fill(letterIndex.begin(), letterIndex.end(), -1);
		for (int left = 0, right = 0; right < length; ++right)
			// substr = [left, right]
		{
			int ascii = static_cast<int>(s[right]);
			int index = letterIndex[ascii];
			if (left <= index)
				left = index + 1;

			letterIndex[ascii] = right;
			maxLength = max(maxLength, right - left + 1);
		}

		return maxLength;
	}
};