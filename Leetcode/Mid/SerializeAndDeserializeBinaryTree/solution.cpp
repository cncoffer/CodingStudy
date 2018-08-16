class Codec {
public:

	// Encodes a tree to a single string.
	string serialize(TreeNode* root) {
		string strResult;
		if (!root) return strResult;
		deque<TreeNode*> dequeTree;
		dequeTree.push_back(root);
		bool bFirst = true;
		while (!dequeTree.empty()) {
			if (bFirst)
				bFirst = false;
			else
				strResult += ",";

			TreeNode *node = dequeTree.front();
			dequeTree.pop_front();
			if (node) {
				strResult += to_string(node->val);
				dequeTree.push_back(node->left);
				dequeTree.push_back(node->right);
			}
			else {
				strResult += "null";
			}
		}
		return strResult;
	}

	// Decodes your encoded data to tree.
	TreeNode* deserialize(string data) {
		vector<string> strTree;
		SplitString(data, strTree, ",");
		if (strTree.empty() || strTree[0] == "null") return NULL;
		TreeNode *root = new TreeNode(atoi(strTree[0].c_str()));
		deque<TreeNode*> dequeTree;
		dequeTree.push_back(root);
		int i = 1, size = strTree.size();
		while (i < size) {
			TreeNode *node = dequeTree.front();
			dequeTree.pop_front();
			if (strTree[i] != "null") {
				node->left = new TreeNode(atoi(strTree[i].c_str()));
				dequeTree.push_back(node->left);
			}
			++i;
			if (i >= size)
				break;
			if (strTree[i] != "null") {
				node->right = new TreeNode(atoi(strTree[i].c_str()));
				dequeTree.push_back(node->right);
			}
			++i;
		}
		return root;
	}
private:
	void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
	{
		std::string::size_type pos1, pos2;
		pos2 = s.find(c);
		pos1 = 0;
		while (std::string::npos != pos2)
		{
			v.push_back(s.substr(pos1, pos2 - pos1));

			pos1 = pos2 + c.size();
			pos2 = s.find(c, pos1);
		}
		if (pos1 != s.length())
			v.push_back(s.substr(pos1));
	}
};