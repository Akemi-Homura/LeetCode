//
// Created by quals on 2019-07-31.
//

#include <vector>
#include <iostream>
#include <map>
#include "Logger.h"

using namespace std;

class Solution {
public:
    /**
     * 分数的大小判断：
     * 判断b1是否为0，b1为0则A一定不小于B
     * 判断b2是否为0，b2为0则A一定小于B（此时b1不为0)
     * 先将分母都设置为正数，a1b2 - a2b1 < 0 则 A < B
     */
    class Fraction {
    public:
        Fraction(int a, int b) : a(a), b(b) {
            if (b < 0) {
                this->a = -a;
                this->b = -b;
            }
        }

        bool operator==(const Fraction &rhs) {
            if (rhs.b == 0 && b == 0) {
                return true;
            }
            if (b == 0 || rhs.b == 0) {
                return false;
            }
            return a * rhs.b == b * rhs.a;
        }


        bool operator<(const Fraction &rhs) const {
            if (b == 0) {
                return false;
            }
            if (rhs.b == 0) {
                return true;
            }
            return a * rhs.b < b * rhs.a;
        }

    private:
        int64_t a, b; // a/b
    };

    int maxPoints(vector<vector<int>> &points) {
        int g_max = 0;
        if (points.size() < 3) {
            return points.size();
        }
        map<Fraction, int> mp;
        for (int i = 0; i < points.size(); i++) {
            mp.clear();
            int l_max = 0;
            int same_point = 0;
            for (int j = i + 1; j < points.size(); j++) {
                int x_diff = points[i][0] - points[j][0];
                int y_diff = points[i][1] - points[j][1];
                if (!x_diff && !y_diff) {
                    same_point++;
                    continue;
                }
                Fraction k(x_diff, y_diff);
//                Fra k = (double) x_diff / y_diff;
                mp[k]++;
                if (mp[k] > l_max) {
                    l_max = mp[k];
                }
            }
            if (l_max + same_point + 1 > g_max) {
                g_max = l_max + same_point + 1;
            }
        }
        return g_max;
    }
};

Solution solution;

void test(vector<vector<int>> &points, int ans) {
    cout << "Points: [";
    for (int i = 0; i < points.size(); i++) {
        cout << "[" << points[i][0] << "," << points[i][1] << "]";
        if (i != points.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;

    cout << "res: " << solution.maxPoints(points) << endl;
    cout << "ans: " << ans << endl;
}

void test_fraction() {
    typedef Solution::Fraction Fraction;
    CHECK(INFO, Fraction(2, 4) == Fraction(4, 8));
    CHECK(INFO, Fraction(1, 0) == Fraction(3, 0));
    CHECK(INFO, Fraction(0, 5) < Fraction(1, 0));
    Fraction a(1,0), b(2,0),c(3,0);

}

int main() {
    test_fraction();
    vector<vector<int>> v1 = {{1, 1},
                              {2, 2},
                              {3, 3}};
    vector<vector<int>> v2 = {{84,  250},
                              {0,   0},
                              {1,   0},
                              {0,   -70},
                              {0,   -70},
                              {1,   -1},
                              {21,  10},
                              {42,  90},
                              {-42, -230}};
    vector<vector<int>> v3 = {{0, 0},
                              {1, 1},
                              {0, 0}};
    vector<vector<int>> v4 = {{1, 1},
                              {1, 1},
                              {1, 1}};
    vector<vector<int>> v5 = {{-4, -4},
                              {-8, -582},
                              {-3, 3},
                              {-9, -651},
                              {9,  591}};
    vector<vector<int>> v6 = {{2,  3},
                              {3,  3},
                              {-5, 3}};
    test(v1, 3);
    test(v2, 6);
    test(v3, 3);
    test(v4, 3);
    test(v5, 3);
    test(v6, 3);
}