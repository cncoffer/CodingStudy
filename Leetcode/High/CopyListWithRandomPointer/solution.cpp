/**
 * Definition for singly-linked list with a random pointer.
 * struct RandomListNode {
 *     int label;
 *     RandomListNode *next, *random;
 *     RandomListNode(int x) : label(x), next(NULL), random(NULL) {}
 * };
 */
class Solution {
public:
    RandomListNode *copyRandomList(RandomListNode *head) {
        if (!head) return NULL;
        RandomListNode result(0);
        RandomListNode *pSrc, *pDst;
        unordered_map<RandomListNode*, RandomListNode*> m;
        pSrc = head;
        pDst = &result;
        while (pSrc) {
            pDst->next = new RandomListNode(pSrc->label);
            m[pSrc] = pDst->next;
            pDst = pDst->next;
            pSrc = pSrc->next;
        }

        pSrc = head;
        pDst = result.next;
        while (pSrc) {
            if (pSrc->random)
                pDst->random = m[pSrc->random];
            
            pDst = pDst->next;
            pSrc = pSrc->next;
        }

        return result.next;
    }
};