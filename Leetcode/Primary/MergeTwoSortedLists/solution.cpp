/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode head(0);
        ListNode *ret = &head;
        ListNode **tmp;
        while (l1 && l2)
        {
            tmp = l1->val < l2->val ? &l1 : &l2;
            ret->next = *tmp;
            *tmp = (*tmp)->next;
            ret = ret->next;
        }
        
        ret->next = l1 ? l1 : l2;
        return head.next;
    }
};