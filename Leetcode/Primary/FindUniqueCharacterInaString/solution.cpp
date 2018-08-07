class Solution {
public:
    int firstUniqChar(string s) {
        unordered_map<char, int> map;
        for (int i = 0; i < s.length(); i++)
        {
            if (map.find(s[i]) != map.end())
                map[s[i]] = -1;
            else
                map[s[i]] = i;
        }

        int ret = s.length();
        for (auto it : map)
        {
            if (it.second != -1 && it.second < ret)
                ret = it.second;
        }
        if (ret >= s.length())
            ret = -1;
        return ret;
    }
};