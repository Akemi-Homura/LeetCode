//
// Created by quals on 2019-07-31.
//

#include <string>
#include <iostream>
#include <unistd.h>
#include <Logger.h>

using namespace std;

class Solution {
public:
    string reverseWords(string s) {
        int j;
        int len = s.length();
        const char *str = s.data();
        string res;
        for (int i = len - 1; i >= 0;i--) {
            for (; str[i] == ' ' && i; i--);
            j = i;
            for (; str[i] != ' ' && i; i--);
            if (i == j && str[i] == ' ')break;
            res.append(s.substr(i + (str[i] == ' '), j - i + (str[i] != ' ')));
            res.append(" ");
        }
        if (res.length() > 1)
            res.erase(res.length() - 1);
        return res;
    }
};

Solution solution;

void test(string input, string ans) {
    cout << "input: " << input << endl;
    cout << "output: " << solution.reverseWords(input) << endl;
    cout << "ans: " << ans << endl;
    cout << endl;
}

int main() {
    test("the sky is blue", "blue is sky the");
    test("  hello world!  ", "world! hello");
    test("a good   example", "example good a");
    test("", "");
    test("a","a");
    test("hello","hello");
}