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
    vector<int> inorderTraversal(TreeNode* root) {
        result.clear();
        recTraversal(root);
        return result;
    }
private:
    vector<int> result;
    void recTraversal(TreeNode *root) {
        if (!root) return;

        recTraversal(root->left);
        result.push_back(root->val);
        recTraversal(root->right);
    }
};


class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        if (!root) return result;

        typedef pair<TreeNode*, int> NodePair;
        stack<NodePair> stTree;
        stTree.push(make_pair(root, 0));
        while (!stTree.empty()) {
            NodePair node = stTree.top();
            stTree.pop();
            TreeNode *p = node.first;
            if (!p) {
                result.push_back(node.second);
                continue;
            }
            if (p->right) stTree.push(make_pair(p->right, 0));
            stTree.push(make_pair(nullptr, p->val));
            if (p->left) stTree.push(make_pair(p->left, 0));
        }
        return result;
    }
};