/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
	vector<vector<int>> levelOrder(TreeNode* root) {
		vector<vector<int>> result;
		if (!root)
			return result;

		deque<pair<TreeNode*, int>> dequeTree; // node, depth
		dequeTree.push_back(make_pair(root, 1));

		int depth = 1;
		vector<int> layer;
		while (!dequeTree.empty())
		{
			auto tmp = dequeTree.front();
			dequeTree.pop_front();
			TreeNode *node = tmp.first;
			if (tmp.second != depth)
			{
				result.push_back(layer);
				layer.clear();
				depth++;
			}

			layer.push_back(node->val);

			if (node->left)
				dequeTree.push_back(make_pair(node->left, depth + 1));
			if (node->right)
				dequeTree.push_back(make_pair(node->right, depth + 1));
		}

		if (!layer.empty())
			result.push_back(layer);

		return result;
	}
};