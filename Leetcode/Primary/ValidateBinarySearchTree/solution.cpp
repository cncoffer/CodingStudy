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
    bool isValidBST(TreeNode* root) {
        isValid = true;
        if (root)
            recCheckAndGetMaxMin(root);
        return isValid;
    }
private:
    // first is min num, second is max num
    pair<int,int> recCheckAndGetMaxMin(TreeNode* root) {
        int cur = root->val;
        pair<int,int> left(cur, cur), right(cur, cur), MaxMin(cur, cur);
        if (root->left)
        {
            left = recCheckAndGetMaxMin(root->left);
            if (left.second >= cur)
                isValid = false;
        }
        if (root->right)
        {
            right = recCheckAndGetMaxMin(root->right);
            if (right.first <= cur)
                isValid = false;
        }
        
        MaxMin.first = min(left.first, min(right.first, cur));
        MaxMin.second = max(left.second, max(right.second, cur));
        return MaxMin;
    }
    bool isValid;
};

class Solution2 {
public:
    bool isValidBST(TreeNode* root, long mn = LONG_MIN, long mx = LONG_MAX) {
        if (!root) return true;
        if (root->val >= mx || root->val <= mn) return false;
        return isValidBST(root->left, mn, root->val) && isValidBST(root->right, root->val, mx);
    }
};