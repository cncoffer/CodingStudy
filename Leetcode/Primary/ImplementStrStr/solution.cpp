class Solution {
public:
	int strStr(string haystack, string needle) {
		int lengthB = needle.length();
		if (!lengthB)
			return 0;

		int lengthA = haystack.length();
		if (!lengthA || lengthA < lengthB)
			return -1;

		int i = 0, j;
		while (i < lengthA)
		{
			for (j = 0; j < lengthB && i + j < lengthA; ++j)
			{
				if (haystack[i + j] != needle[j]) // not match
					break;
			}
			if (j == lengthB) // find it
				return i;
			else
			{
				if (i + lengthB >= lengthA)
					break;

				auto ret = needle.rfind(haystack[i + lengthB]);
				if (ret == string::npos)
					i += lengthB + 1;
				else
					i += lengthB - ret;
			}
		}

		return -1;
	}
};

class Solution2 { // KMP
public:
	int strStr(string haystack, string needle) {
		if (needle.empty())
			return 0;

		int i = 0, j = 0;
		int n = haystack.length(), m = needle.length();
		auto next = GetNext(needle);
		while (i < n && j < m)
		{
			if (j == -1 || haystack[i] == needle[j]) {
				++i;
				++j;
			}
			else
			{
				j = next[j];
			}
		}
		if (j == m)
			return i - j;
		else
			return -1;

	}
private:
	vector<int> GetNext(string n)
	{
		vector<int> next(n.length());
		next[0] = -1;
		int j = 0, k = -1;
		while (j < n.length() - 1)
		{
			if (k == -1 || n[j] == n[k])
				next[++j] = ++k;
			else
				k = next[k];
		}
		return next;
	}
};