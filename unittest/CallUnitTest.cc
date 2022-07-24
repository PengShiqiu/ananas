#include <typeinfo>
#include "future/Helper.h"
#include "gtest/gtest.h"

using namespace ananas::internal;

long f_void(void) {
    return 1;
}

int f_int(int a) {
    return a++;
}

void f_lvalue_int(int& a) {
    ++a;
}

void f_rvalue_int(int&& a) {
    ++a;
}

TEST(call, CallableResult) {
    EXPECT_TRUE(typeid(long) == typeid(CallableResult<decltype(f_void), int>::Arg::Type));
    EXPECT_TRUE(typeid(int) == typeid(CallableResult<decltype(f_int), int>::Arg::Type));

    auto lbd = []()-> int {
        return 0;
    };

    EXPECT_TRUE(typeid(int) == typeid(CallableResult<decltype(lbd), void>::Arg::Type));
    EXPECT_TRUE(typeid(int) == typeid(CallableResult<decltype(lbd), int>::Arg::Type));
}

TEST(call, CanCallWith) {
    auto vlbd = []() {};
    EXPECT_TRUE((CanCallWith<decltype(vlbd)>::value));
    EXPECT_FALSE((CanCallWith<decltype(vlbd), int>::value));

    EXPECT_TRUE((CanCallWith<decltype(f_void)>::value));
    EXPECT_FALSE((CanCallWith<decltype(f_void), int>::value));

    EXPECT_TRUE((CanCallWith<decltype(f_int), int>::value));
    EXPECT_TRUE((CanCallWith<decltype(f_int), int&>::value));
    EXPECT_TRUE((CanCallWith<decltype(f_int), int&&>::value));
    EXPECT_FALSE((CanCallWith<decltype(f_int)>::value));

    EXPECT_FALSE((CanCallWith<decltype(f_lvalue_int), int>::value));
    EXPECT_TRUE((CanCallWith<decltype(f_lvalue_int), int&>::value));
    EXPECT_FALSE((CanCallWith<decltype(f_lvalue_int), int&&>::value));
    EXPECT_FALSE((CanCallWith<decltype(f_lvalue_int)>::value));

    EXPECT_TRUE((CanCallWith<decltype(f_rvalue_int), int>::value));
    EXPECT_FALSE((CanCallWith<decltype(f_rvalue_int)>::value));
    EXPECT_FALSE((CanCallWith<decltype(f_rvalue_int), int&>::value));
    EXPECT_TRUE((CanCallWith<decltype(f_rvalue_int), int&&>::value));
}
