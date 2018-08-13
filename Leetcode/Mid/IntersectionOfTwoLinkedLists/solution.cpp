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
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (!headA || !headB) return NULL;

        int lengthA, lengthB;
        ListNode *pa, *pb;
        lengthA = lengthB = 0;
        pa = headA;
        pb = headB;
        while (pa->next) {
            pa = pa->next;
            ++lengthA;
        }
        while (pb->next) {
            pb = pb->next;
            ++lengthB;
        }
        if (pa != pb) return NULL;

        pa = headA;
        pb = headB;
        if (lengthA > lengthB)
            for (int i = 0; i < lengthA - lengthB; ++i) pa = pa->next;
        else
            for (int i = 0; i < lengthB - lengthA; ++i) pb = pb->next;
        while(pa != pb) {
            pa = pa->next;
            pb = pb->next;
        }
        return pa;
    }
};