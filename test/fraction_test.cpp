//
// Created by quals on 2019-08-03.
//

#include <fraction.h>
#include <gtest/gtest.h>
#include <vector>
#include <array>

GTEST_TEST(FractionTest, GcdTest) {
    std::vector<std::array<int, 3>> testCase = {
            {12, 44,  4},
            {11, 99,  11},
            {14, 100, 2}
    };

    for (auto &singleCase:testCase) {
        ASSERT_EQ(singleCase[2], gcd(singleCase[0], singleCase[1]));
        ASSERT_EQ(singleCase[2], gcd(singleCase[1], singleCase[0]));
    }
}

GTEST_TEST(FractionTest, Constructor) {
    std::vector<std::array<int, 3>> testCase = {
            {12, 44,  4},
            {11, 99,  11},
            {14, 100, 2}
    };

    for (auto &singleCase:testCase) {
        const fraction a(singleCase[0], singleCase[1]);
        const fraction c(-singleCase[0], singleCase[1]);
        const fraction b(singleCase[0] / singleCase[2], singleCase[1] / singleCase[2]);
        const fraction d(singleCase[0] / singleCase[2], -singleCase[1] / singleCase[2]);

        ASSERT_EQ(a, b);
        ASSERT_EQ(a.GetA(), b.GetA());
        ASSERT_EQ(a.GetB(), b.GetB());
        ASSERT_EQ(a.GetA(), singleCase[0] / singleCase[2]);
        ASSERT_EQ(a.GetB(), singleCase[1] / singleCase[2]);

        ASSERT_EQ(c, d);
        ASSERT_EQ(c.GetA(), d.GetA());
        ASSERT_EQ(c.GetB(), d.GetB());
        ASSERT_EQ(c.GetA(), -singleCase[0] / singleCase[2]) << "c is " << c << "\n" << "d is " << d << std::endl;
        ASSERT_EQ(c.GetB(), singleCase[1] / singleCase[2]);
    }

    ASSERT_EQ(fraction(0, 55), fraction(0, 124542));
    ASSERT_EQ(fraction(13, 0), fraction(45, 0));

    for (int i = 1; i < 100; i++) {
        const fraction a(i);
        const fraction b(-i);
        const fraction c(i, 1);
        const fraction d(i, -1);
        ASSERT_EQ(a.GetA(), i);
        ASSERT_EQ(a.GetB(), 1);

        ASSERT_EQ(b.GetA(), -i);
        ASSERT_EQ(b.GetB(), 1);

        ASSERT_EQ(a, c);
        ASSERT_EQ(b, d);
    }

    ASSERT_EQ(fraction(), fraction(0, 3));
}

GTEST_TEST(FractionTest, Operator) {
    fraction f_3_4(3, 4);
    fraction f_1_0(1, 0);
    fraction f_2_3(2, 3);
    fraction f_o3_4(-3, 4);
    fraction f_0;

    ASSERT_GT(f_3_4, f_2_3);
    ASSERT_GE(f_2_3, f_2_3);
    ASSERT_GE(f_3_4, f_2_3);

    ASSERT_LT(f_2_3, f_3_4);
    ASSERT_LE(f_2_3, f_3_4);
    ASSERT_LE(f_2_3, f_2_3);

    ASSERT_GE(f_1_0, f_2_3);
    ASSERT_GE(f_1_0, f_3_4);
    ASSERT_GE(f_1_0, f_o3_4);
    ASSERT_GE(f_0, f_o3_4);

    ASSERT_LE(f_o3_4, f_0);

    ASSERT_EQ(fraction(8, 15), (fraction(1, 5) + fraction(1, 3)));
    ASSERT_EQ(fraction(1657, 385), fraction(1, 5) + fraction(1, 3) + fraction(4) + fraction() - fraction(53, 231));
    ASSERT_EQ(-fraction(4697, 265), (fraction(1, 5) * fraction(1, 3) + fraction(4) + fraction()) / -fraction(53, 231));

}