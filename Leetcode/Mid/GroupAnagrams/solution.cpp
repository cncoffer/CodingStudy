class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> ret;
        if (strs.empty()) return ret;
        
        for (auto &str : strs) {
            bool notfound = true;
            for (auto &keylist : ret) {
                if (strEqual(keylist[0], str)) {
                    keylist.push_back(str);
                    notfound = false;
                    break;
                }
            }
            if (notfound)
            {
                vector<string> tmp = {str};
                ret.push_back(tmp);
            }
        }
        return ret;
    }
private:
    #define LetterNum 26
    vector<int> convertToHash(string s) {
        vector<int> vt(LetterNum);
        for (auto ele : s)
        {
            ++vt[ele - 'a'];
        }
        return vt;
    }
    bool HashEqual(vector<int> stra, vector<int> strb) {
        for (int i = 0; i < LetterNum; ++i)
        {
            if (stra[i] != strb[i]) return false;
        }
        return true;
    }
    bool strEqual(string stra, string strb) {
        return HashEqual(convertToHash(stra), convertToHash(strb));
    }
};

// solution2
class Solution {
public:
	vector<vector<string>> groupAnagrams(vector<string>& strs) {
		vector<vector<string>> ret;
		if (strs.empty()) return ret;
		typedef pair<string, vector<int>> keylist;
		typedef unordered_map<int, vector<vector<keylist>>> keymap;
		keymap tempRet;
		for (string &str : strs) {
			int hash;
			vector<int> hashArray;
			convertToHash(str, hash, hashArray);
			keymap::iterator matchKey = tempRet.find(hash);
			if (matchKey != tempRet.end()) {
				int notfound = true;
				for (auto &keys : matchKey->second) {
					if (cmpHashArray(keys[0].second, hashArray))
					{
						keys.push_back(make_pair(str, hashArray));
						notfound = false;
						break;
					}
				}
				if (notfound)
				{
					vector<keylist> vtKeylist = { make_pair(str, hashArray) };
					matchKey->second.push_back(vtKeylist);
				}
			}
			else {
				vector<keylist> vtKeylist = { make_pair(str, hashArray) };
				vector<vector<keylist>> vtKeymapSecond = {vtKeylist};
				tempRet[hash] = vtKeymapSecond;
			}
		}

		for (auto &vtKeymapSecond : tempRet) {
			for (vector<keylist> &vtkeylist : vtKeymapSecond.second) {
				vector<string> vtStr;
				for (keylist &keys : vtkeylist)
					vtStr.push_back(keys.first);
				ret.push_back(vtStr);
			}
		}

		return ret;
	}
private:
#define LetterNum 26
	void convertToHash(string s, int &hash, vector<int> &hashArray) {
		hash = 0;
		hashArray.resize(LetterNum);
		for (auto ele : s)
		{
			++hashArray[ele - 'a'];
			hash |= 1 << (ele - 'a');
		}
	}
	bool cmpHashArray(vector<int> &stra, vector<int> &strb)
	{
		for (int i = 0; i < LetterNum; ++i)
			if (stra[i] != strb[i]) return false;
		return true;
	}
};

// solution3
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string,vector<string>> mps;
        vector<vector<string>> outs;
        for(string s : strs){
            string ss = s;
            sort(s.begin(),s.end());
            mps[s].push_back(ss);
        }
        for(auto m:mps){
            outs.push_back(m.second);
        }
        return outs;
    }
};