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
	bool isPalindrome(ListNode* head) {
		if (!head)
			return true;

		vector<int> vt;
		while (head)
		{
			vt.push_back(head->val);
			head = head->next;
		}

		auto left = vt.begin();
		auto right = vt.end() - 1;
		while (left < right)
		{
			if (*(left++) != *(right--))
				return false;
		}

		return true;
	}
};

class Solution2 {
public:
	bool isPalindrome(ListNode* head) {
		if (!head)
			return true;

		const int shiftDigit = sizeof(int) * 8;
		int size = 0;
		ListNode *temp = head;
		while (temp=temp->next) size++;

		if (size == 0)
			return true;

		temp = head;
		int count = 0;
		int midCount = size / 2;
		long long cmp = 0, midNum;
		while (temp)
		{
			long long val;
			if (count <= midCount)
				midNum = val = ((long long)count << shiftDigit) + static_cast<long long>(temp->val);
			else
				val = ((long long)(size - count) << shiftDigit) + static_cast<long long>(temp->val);

			cmp ^= val;
			count++;
			temp = temp->next;
		}

		if (size % 2 != 0)  // even
			midNum = 0;

		return cmp == midNum;
	}
};

class Solution3 {
public:
	bool isPalindrome(ListNode* head) {
		if (!head)
			return true;

		int size = 0;
		ListNode *temp = head;
		while (temp = temp->next) size++;
		if (size == 0)
			return true;

		int midpos = size / 2 + 1;
		int count = 0;
		temp = head;
		while (temp) // get mid node ptr
		{
			if (count == midpos)
				break;
			count++;
			temp = temp->next;
		}
		temp = reverseList(temp);
		while (temp && head)
		{
			if (temp->val != head->val)
				return false;

			temp = temp->next;
			head = head->next;
		}

		return true;
	}

private:
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