//
// Created by xiong on 7/13/2019.
//

#include <stdio.h>

struct ListNode {
    int val;
    ListNode *next;

    ListNode(int x) : val(x), next(nullptr) {}
};

/*
class Solution {
public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        ListNode *head = new ListNode(-1);
        ListNode *now = head;
        int x = 0; // 进位
        int v; // 当前位的值
        while (l1 || l2) {
            v = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + x;
            if (v >= 10) {
                v -= 10;
                x = 1;
            } else {
                x = 0;
            }
            now->next = new ListNode(v);
            now = now->next;

            l1 = l1 ? l1->next : nullptr;
            l2 = l2 ? l2->next : nullptr;
        }
        if(x){
            now->next = new ListNode(x);
        }
        ListNode *ans = head->next;
        delete head;
        return ans;
    }
};
*/

class Solution {
public:
    int LenCompare(ListNode *l1, ListNode *l2) {
        while (l1 && l2) {
            l1 = l1->next;
            l2 = l2->next;
        }
        return l1 ? 1 : 0;
    }

    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        ListNode *now = LenCompare(l1, l2) ? l1 : l2;
        ListNode *ans = now;
        ListNode *pre;
        int x = 0; // 进位
        int v; // 当前位的值
        while (l1 || l2) {
            v = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + x;
            if (v >= 10) {
                v -= 10;
                x = 1;
            } else {
                x = 0;
            }
            now->val = v;
            pre = now;
            now = now->next;

            l1 = l1 ? l1->next : nullptr;
            l2 = l2 ? l2->next : nullptr;
        }
        if (x) {
            pre->next = new ListNode(x);
        }
        return ans;
    }
};

ListNode *num2list(int num) {
    if (!num) {
        return new ListNode(0);
    }
    ListNode *l = new ListNode(-1);
    ListNode *now = l;
    while (num) {
        now->next = new ListNode(num % 10);
        now = now->next;
        num /= 10;
    }
    ListNode *ans = l->next;
    delete l;
    return ans;
}

void PrintList(ListNode *l) {
    printf("(");
    while (l) {
        printf("%d%s", l->val, l->next ? " -> " : ")\n");
        l = l->next;
    }
}

Solution solution;

bool ValCompare(ListNode *l1, ListNode *l2) {
    while (l1 && l2) {
        if (l1->val != l2->val) {
            return false;
        }
        l1 = l1->next;
        l2 = l2->next;
    }
    return !l1 && !l2;
}

void test(int a, int b) {
    printf("Test %d + %d\n", a, b);
    ListNode *l1 = num2list(a);
    ListNode *l2 = num2list(b);
    ListNode *res = solution.addTwoNumbers(l1, l2);
    ListNode *ans = num2list(a + b);
    printf("l1: ");
    PrintList(l1);
    printf("l2: ");
    PrintList(l2);
    printf("res: ");
    PrintList(res);
    printf("ans: ");
    PrintList(ans);
    printf("%s\n", ValCompare(res, ans) ? "Accept" : "Error");
    printf("------------------------\n");
}

int main() {
    test(0, 1);
    test(123, 456);
    test(342, 465);
    test(10001, 1);
    test(1, 999);
}