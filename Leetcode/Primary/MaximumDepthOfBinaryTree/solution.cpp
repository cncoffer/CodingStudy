/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution { // 递归
public:
    int maxDepth(TreeNode* root) {
        return recGetChildDepth(root);
    }
private:
    int recGetChildDepth(TreeNode* root) {
        if (!root) return 0;
        
        return max(recGetChildDepth(root->left), recGetChildDepth(root->right)) + 1;
    }
};

class Solution2 { // 层序遍历
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;

        int depth;
        treeDeque.push_back(make_pair(root, 1));

        while (!treeDeque.empty())
        {
            auto node = treeDeque.front();
            treeDeque.pop_front();
            depth = node.second;
            if (node.first->left)
                treeDeque.push_back(make_pair(node.first->left, depth+1));
            if (node.first->right)
                treeDeque.push_back(make_pair(node.first->right, depth+1));
        }
        return depth;
    }
private:
    deque<pair<TreeNode*, int>> treeDeque;
};