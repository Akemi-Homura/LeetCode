//
// Created by xiong on 7/14/2019.
//

#include <string>
#include <iostream>
#include <string.h>
#include <set>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Solution {
public:

    int lengthOfLongestSubstring(string s) {
        int pos[256];
        int ans = 0, pre = 0;
        memset(pos, -1, sizeof(pos));
        for (int i = 0; i < s.length(); i++) {
            if (pos[(unsigned char) s[i]] != -1) {
                ans = max(ans, i - pre);
                pre = max(pre, pos[(unsigned char) s[i]])+1;
            } else {
                ans = max(ans, i - pre + 1);
            }

            pos[(unsigned char) s[i]] = i;
        }

        return ans;
    }
};

Solution solution;

void test(string s, int ans) {
    int res = solution.lengthOfLongestSubstring(s);
    ostream &os = res == ans ? cout : cerr;

    os << s << "\nres: " << res << "\nans: " << ans << endl;
}

int main() {
    test("abcabcbb", 3);
    test("bbbbb", 1);
    test("pwwkew", 3);
    test("abba", 2);
    test(" ", 1);
    test("dvdf", 3);
    test("tmmzuxt", 5);
}