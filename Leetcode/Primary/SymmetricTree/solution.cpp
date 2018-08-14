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
    bool isSymmetric(TreeNode* root) {
        if (!root)
            return true;

        treeDeque.push_back(root, 1);

    }
private:
    deque<TreeNode*> layer;
    deque<TreeNode*, int> treeDeque;
};

class Solution2 { // 递归
public:
    bool isSymmetric(TreeNode* root) {
        if (!root)
            return true;

        return checkChildTree(root->left, root->right);
    }
private:
    bool checkChildTree(TreeNode *leftTree, TreeNode *rightTree) {
        if (!leftTree && !rightTree)
            return true;

        if (!leftTree || !rightTree)
            return false;

        return leftTree->val == rightTree->val
            && checkChildTree(leftTree->left, rightTree->right)
            && checkChildTree(leftTree->right, rightTree->left);
    }
};

class Solution3 {
public:
	bool isSymmetric(TreeNode* root) {
		if (!root)
			return true;

		stack<TreeNode*> stackLeft, stackRight;
		stackLeft.push(root->left);
		stackRight.push(root->right);
		while (!stackLeft.empty() && !stackRight.empty())
		{
			TreeNode *left = stackLeft.top();
			stackLeft.pop();
			TreeNode *right = stackRight.top();
			stackRight.pop();

			if (!left && !right)
				continue;

			if (!left || !right || left->val != right->val)
				return false;

			stackLeft.push(left->right);
			stackLeft.push(left->left);
			stackRight.push(right->left);
			stackRight.push(right->right);
		}

		return stackLeft.empty() && stackRight.empty();
	}
};