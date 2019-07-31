//
// Created by quals on 2019-07-16.
//

#include <string.h>
#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    bool checkNumber(const string &s, int &pos) {
        int e_pos = -1;
        int e_num = 0;
        int i_ed;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == 'e') {
                e_num++;
                e_pos = i;
            }
        }
        for (i_ed = pos; s[i_ed] != ' ' && i_ed < s.length(); i_ed++);
        if (e_num > 1) {
            return false;
        }
        if (e_pos != -1) {
            return /*'e' 不能为第一个字符 */pos != e_pos && checkFloat(s, pos, e_pos) && /* 'e' 不能为最后一个字符 */e_pos + 1 != i_ed &&
                                    checkInteger(s, e_pos + 1, i_ed) && checkBlack(s, i_ed, s.length());
        }
        return checkFloat(s, pos, i_ed) && checkBlack(s, i_ed, s.length());
    }

    bool checkBlack(const string &s, int st, int ed) {
        for (int i = st; i < ed; i++) {
            if (s[i] != ' ') {
                return false;
            }
        }
        return true;
    }

    bool checkFloat(const string &s, int st, int ed) {
        int dot_pos = -1;
        int dot_num = 0;
        if (st == ed) {
            return false;
        }
        for (int i = st; i < ed; i++) {
            if (s[i] == '.') {
                dot_pos = i;
                ++dot_num;
            }
        }
        if (dot_num > 1) {
            return false;
        }
        if (dot_pos != -1) {
            return checkInteger(s, st, dot_pos, true) && checkDigit(s, dot_pos + 1, ed) &&
                   /* 小数点前后必须有数字 */checkAroundDot(s, st, ed, dot_pos);
        }
        return st != ed && checkInteger(s, st, ed);
    }

    bool checkAroundDot(const string &s, int st, int ed, int dot_pos) {
        return (dot_pos > st && isdigit(s[dot_pos - 1])) || (dot_pos < ed - 1 && isdigit(s[dot_pos + 1]));
    }

    bool checkDigit(const string &s, int st, int ed) {
        for (int i = st; i < ed; i++) {
            if (!isdigit(s[i])) {
                return false;
            }
        }
        return true;
    }

    bool checkInteger(const string &s, int st, int ed, bool canBeEmpty = false) {
        if (s[st] == '+' || s[st] == '-') {
            st++;
            if (st == ed && !canBeEmpty) {
                return false;
            }
        }
        return checkDigit(s, st, ed);
    }

    bool isNumber(string s) {
        // 去掉前置空格
        int pos;
        for (pos = 0; s[pos] == ' ' && pos < s.length(); pos++);
        return checkNumber(s, pos);
    }
};

Solution solution;

void test(string s, bool ans) {
    bool res = solution.isNumber(s);
    cout << s << "\nres: " << res << "\nans: " << ans << endl;
}

int main() {
    test("0", true);
    test(" 0.1 ", true);
    test("abc", false);
    test("1 a", false);
    test("2e10", true);
    test(" -90e3   ", true);
    test(" 1e", false);
    test("e3", false);
    test(" 6e-1", true);
    test(" 99e2.5 ", false);
    test("53.5e93", true);
    test(" --6 ", false);
    test("-+3", false);
    test("95a54e53", false);
    test(".1", true);
    test(".", false);
    test("3.", true);
    test(" .", false);
    test("  .    ", false);
    test("0..", false);
    test("   .1  ", true);
    test("  2.   ", true);
    test("+.8", true);
    test("46.e3", true);
    test("4e+", false);
    test("4e+1", true);
    test("-.", false);
    test("-.1", true);
}