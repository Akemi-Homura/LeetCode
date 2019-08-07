//
// Created by quals on 2019-08-07.
//

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;

    explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    int calItemNum(ListNode *pNode) {
        int sum = 0;
        for (auto now = pNode; now; now = now->next, ++sum);
        return sum;
    }

    vector<int> nextLargerNodes(ListNode *head) {
        int n = calItemNum(head);
        stack<pair<int, int>> s;
        vector<int> res(n);
        s.push(make_pair(0, head->val));
        head = head->next;
        for (int i = 1; head; i++, head = head->next) {
            while (!s.empty() && head->val > s.top().second) {
                res[s.top().first] = head->val;
                s.pop();
            }
            s.push(make_pair(i, head->val));
        }
        while (s.empty()) {
            res[s.top().first] = 0;
            s.pop();
        }
        return res;
    }
};

Solution solution;

ListNode *Vector2List(vector<int> vec) {
    ListNode *head = new ListNode(vec[0]);
    ListNode *now = head;
    for (int i = 1; i < vec.size(); i++) {
        now->next = new ListNode(vec[i]);
        now = now->next;
    }
    return head;
}

void PrintVector(vector<int> vec) {
    cout << "[";
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << (i == vec.size() - 1 ? "]" : ", ");
    }
    cout << endl;
}

void test(vector<int> vec, vector<int> ans) {
    auto res = solution.nextLargerNodes(Vector2List(vec));
    PrintVector(vec);
    cout << "res: ";
    PrintVector(res);
    cout << "ans: ";
    PrintVector(ans);
    cout << endl;
}


int main() {
    test({2, 1, 5}, {5, 5, 0});
    test({2, 7, 4, 3, 5}, {7, 0, 5, 5, 0});
    test({1, 7, 5, 1, 9, 2, 5, 1}, {7, 9, 9, 9, 0, 5, 0, 0});
}