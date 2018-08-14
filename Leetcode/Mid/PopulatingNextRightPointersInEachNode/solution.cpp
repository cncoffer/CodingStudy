/**
 * Definition for binary tree with next pointer.
 * struct TreeLinkNode {
 *  int val;
 *  TreeLinkNode *left, *right, *next;
 *  TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
 * };
 */
class Solution {
public:
    void connect(TreeLinkNode *root) {
        recConnect(root, NULL);
    }
private:
    void recConnect(TreeLinkNode *node, TreeLinkNode *rightnode) {
        if (!node) return;
        node->next = rightnode;
        if (node->left) recConnect(node->left, node->right);
        if (node->right) recConnect(node->right, rightnode ? rightnode->left : NULL);
    }
};