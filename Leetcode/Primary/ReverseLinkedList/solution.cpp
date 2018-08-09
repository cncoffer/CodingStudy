/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution { // iteratively 
public:
	ListNode* reverseList(ListNode* head) {
		ListNode* p = head, *temp;
		ListNode retNode(0);
		while (p) {
			temp = retNode.next;
			retNode.next = p;
			p = p->next;
			retNode.next->next = temp;
		}

		return retNode.next;
	}
};

class Solution2 { // recursively
public:
	ListNode* reverseList(ListNode* head) {
		return recSetHeadtoTail(head, NULL);
	}

	ListNode* recSetHeadtoTail(ListNode *head, ListNode **tail /*out*/)
	{
		if (!head)
			return NULL;

		if (tail)
			*tail = head;
		if (!(head->next))
		{
			return head;
		}

		ListNode *ret, *tmp_tail;
		ret = recSetHeadtoTail(head->next, &tmp_tail);
		if (tmp_tail)
			tmp_tail->next = head;

		head->next = NULL;
		return ret;
	}
};