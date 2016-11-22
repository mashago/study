/*
You are given two linked lists representing two non-negative numbers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8

Tags: Linked List, Math
*/

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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *r = new ListNode(0);

        int t = 0;
        ListNode *rp = r;
        ListNode *last = r;
        while (l1 != NULL || l2 != NULL) {
            int n1 = (l1 == NULL ? 0 : l1->val);
            int n2 = (l2 == NULL ? 0 : l2->val);
            
            rp->val = (t + n1 + n2) % 10;
            t = (t + n1 + n2) / 10;
            
            l1 = l1 == NULL ? NULL : l1->next;
            l2 = l2 == NULL ? NULL : l2->next;
            rp->next = new ListNode(0);
            last = rp;
            rp = rp->next;
        }
        if (t != 0) {
            rp->val = t;
        } else {
            delete(last->next);
            last->next = NULL;
        }
        
        return r;
    }
};

