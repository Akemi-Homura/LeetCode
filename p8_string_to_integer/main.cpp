//
// Created by quals on 2019-07-15.
//

#include <chrono>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <string>
#include <iostream>
#include <ctype.h>

using namespace std::chrono;
using namespace std;


class Solution {
public:
    int myAtoi(string str) {
        // 去掉前置的空格
        int pos;
        for (pos = 0; pos < str.length() && str[pos] == ' '; pos++);
        int64_t ans = 0;
        bool flag = false;
        if (pos == str.length()) {
            return ans;
        }
        if (str[pos] == '-') {
            flag = true;
            pos++;
        }else if (str[pos] == '+') {
            pos++;
        }
        for (; isdigit(str[pos]) && pos < str.length(); pos++) {
            ans = ans * 10 + str[pos] - '0';
            if (ans >INT_MAX){
                break;
            }
        }
        if (flag) ans = -ans;
        if (ans > INT_MAX) {
            ans = INT_MAX;
        }
        if (ans < INT_MIN) {
            ans = INT_MIN;
        }
        return ans;
    }
};

Solution solution;

void test(string str, int ans) {
    int res = solution.myAtoi(str);
    cout << str << "\nres: " << res << "\nans: " << ans << std::endl;
}

int main() {
    test("42", 42);
    test("   -42", -42);
    test("4193 with words", 4193);
    test("-+1",0);
    test("+-1",0);
}