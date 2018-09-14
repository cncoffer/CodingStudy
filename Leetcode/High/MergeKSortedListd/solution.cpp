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
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return NULL;
        deque<ListNode*> listQueue;
        for (auto node : lists)
            listQueue.push_back(node);
        
        while(listQueue.size() >= 2) {
            ListNode *list1 = listQueue.front();
            listQueue.pop_front();
            ListNode *list2 = listQueue.front();
            listQueue.pop_front();
            ListNode *result = mergeTwoLists(list1, list2);
            listQueue.push_back(result);
        }
        return listQueue.front();
    }
private:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if (!list1) return list2;
        if (!list2) return list1;
        ListNode head(0);
        ListNode *p = &head;
        while (list1 || list2) {
            if (!list1 || (list2 && list1->val > list2->val) {
                p->next = list2;
                list2 = list2->next;
                p = p->next;
                p->next = NULL;
            }
            else {
                p->next = list1;
                list1 = list1->next;
                p = p->next;
                p->next = NULL;
            }
        }
        return head->next;
    }
};