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
    ListNode* sortList(ListNode* head) {
        if (!head || !(head->next)) return head;
        vector<int> vtList;
        ListNode *p = head;
        while (p) {
            vtList.push_back(p->val);
            p = p->next;
        }
        sort(vtList.begin(), vtList.end());
        p = head;
        auto it = vtList.begin();
        while (p) {
            p->val = *it;
            p = p->next;
            it++;
        }
        return head;
    }
};

// solution2
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !(head->next)) return head;
        ListNode *mid, *right, *last_mid;
        mid = right = head;
        while (right) {
            right = right->next;
            if (right)
                right = right->next;
            last_mid = mid;
            mid = mid->next;
        }
        last_mid->next = NULL;
        return mergeList(sortList(head), sortList(mid));
    }
private:
    ListNode* mergeList(ListNode* list1, ListNode* list2) {
        if (!list1) return list2;
        if (!list2) return list1;

        ListNode head(0);
        ListNode *p = &head;
        while (list1 || list2) {
            if (!list2 || (list1 && list1->val < list2->val)) {
                p->next = list1;
                list1 = list1->next;
                p = p->next;
                p->next = NULL;
            }
            else {
                p->next = list2;
                list2 = list2->next;
                p = p->next;
                p->next = NULL;
            }
        }
        return head.next;
    }
};