//
// Created by quals on 2019-08-03.
//

#ifndef LEETCODE_FRACTION_H
#define LEETCODE_FRACTION_H
/**
 * 分数类
 * 分子、分母都为64位有符号整数
 * 支持的操作：
 * 1. 加减乘除
 * 2. 比较操作
 */
#include <iostream>
#include <type_traits>

long gcd(long a, long b) {
    while (b) {
        long r = a % b;
        a = b;
        b = r;
    }
    return a;
}

class fraction {
public:
    explicit fraction(long a, long b) : A(a), B(b) {
        adjust();
        simplify();
    }

    explicit fraction() : fraction(0, 1) {}

    explicit fraction(long a) : fraction(a, 1) {}

    fraction(const fraction &rhs) = default;

    fraction(fraction &&rhs) noexcept : A(rhs.A), B(rhs.B) {}

    fraction &adjust() {
        if (B < 0) {
            A = -A;
            B = -B;
        }
        return *this;
    }

    fraction &simplify() {
        if (B) {
            long g = gcd(A, B);
            if (g < 0)g = -g;
            A /= g;
            B /= g;
        }
        return *this;
    }

    fraction &operator=(const fraction &lhs) = default;

    fraction &operator=(fraction &&lhs) noexcept = default;

    fraction &operator=(std::initializer_list<long> li) {
        auto size = li.size();
        assert(size <= 2);
        auto begin = li.begin();
        switch (size) {
            case 0:
                A = 0;
                B = 1;
                break;
            case 1:
                A = *begin;
                B = 1;
                break;
            case 2:
                A = *begin;
                B = *(++begin);
                break;
            default:
                throw std::exception();
        }
        return *this;
    }


    long GetA() {
        return A;
    }

    long GetB() {
        return B;
    }

    long GetA() const {
        return A;
    }

    long GetB() const {
        return B;
    }

    friend std::ostream &operator<<(std::ostream &os, const fraction &f);

    friend std::ostream &operator<<(std::ostream &os, fraction &&f);

    friend fraction operator-(const fraction &lhs);

    friend fraction operator+(const fraction &lhs, const fraction &rhs);

    friend fraction operator-(const fraction &lhs, const fraction &rhs);

    friend fraction operator*(const fraction &lhs, const fraction &rhs);

    friend fraction operator/(const fraction &lhs, const fraction &rhs);

    friend fraction &operator+=(fraction &lhs, const fraction &rhs);

    friend fraction &operator-=(fraction &lhs, const fraction &rhs);

    friend fraction &operator*=(fraction &lhs, const fraction &rhs);

    friend fraction &operator/=(fraction &lhs, const fraction &rhs);


    friend bool operator<(const fraction &lhs, const fraction &rhs);

    friend bool operator==(const fraction &lhs, const fraction &rhs);

    friend bool operator!=(const fraction &lhs, const fraction &rhs);

    friend bool operator>(const fraction &lhs, const fraction &rhs);

    friend bool operator<=(const fraction &lhs, const fraction &rhs);

    friend bool operator>=(const fraction &lhs, const fraction &rhs);

private:
    long A, B;
};

bool operator<(const fraction &lhs, const fraction &rhs) {
    if (lhs.B == 0) {
        return false;
    }
    if (rhs.B == 0) {
        return true;
    }
    return lhs.A * rhs.B < lhs.B * rhs.A;
}

bool operator==(const fraction &lhs, const fraction &rhs) {
    if (lhs.B == 0 && rhs.B == 0) {
        return true;
    }
    if (lhs.B == 0 || rhs.B == 0) {
        return false;
    }
    return lhs.A * rhs.B == lhs.B * rhs.A;
}

bool operator!=(const fraction &lhs, const fraction &rhs) {
    return !(lhs == rhs);
}

bool operator>(const fraction &lhs, const fraction &rhs) {
    return rhs < lhs;
}

bool operator<=(const fraction &lhs, const fraction &rhs) {
    return !(rhs < lhs);
}

bool operator>=(const fraction &lhs, const fraction &rhs) {
    return !(lhs < rhs);
}

fraction operator+(const fraction &lhs, const fraction &rhs) {
    return fraction(lhs.A * rhs.B + rhs.A * lhs.B, lhs.B * rhs.B).simplify().adjust();
}

fraction operator-(const fraction &lhs, const fraction &rhs) {
    return lhs + (-rhs);
}

fraction operator*(const fraction &lhs, const fraction &rhs) {
    return fraction(lhs.A * rhs.A, lhs.B * rhs.B).simplify().adjust();
}

fraction operator/(const fraction &lhs, const fraction &rhs) {
    return fraction(lhs.A * rhs.B, lhs.B * rhs.A).simplify().adjust();
}

fraction operator-(const fraction &lhs) {
    return fraction(lhs.A, -lhs.B);
}

fraction &operator+=(fraction &lhs, const fraction &rhs) {
    lhs = lhs + rhs;
    return lhs;
}

fraction &operator-=(fraction &lhs, const fraction &rhs) {
    lhs = lhs - rhs;
    return lhs;
}

fraction &operator*=(fraction &lhs, const fraction &rhs) {
    lhs = lhs * rhs;
    return lhs;
}

fraction &operator/=(fraction &lhs, const fraction &rhs) {
    lhs = lhs / rhs;
    return lhs;
}

std::ostream &operator<<(std::ostream &os, const fraction &f) {
    os << "(" << f.A << ", " << f.B << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, fraction &&f) {
    os << "(" << f.A << ", " << f.B << ")";
    return os;
}

#endif //LEETCODE_FRACTION_H
