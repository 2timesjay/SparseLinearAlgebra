#include <gtest/gtest.h>
#include <list>
#include "SparseVector.h"

using namespace std;

TEST(Add, DisjointIdLists) {
    SparseVector<int> c = SparseVector<int>(52);
    c.push_back(3, 5);
    c.push_back(11, 4);
    c.push_back(24, 3);

    SparseVector<int> d = SparseVector<int>(52);
    d.push_back(8, 2);
    d.push_back(30, 1);

    SparseVector<int> e = c + d;

    IVIterator<int> iv_it = e.iv_list.begin();
    ASSERT_EQ(5, (*iv_it).second);
    ++iv_it;
    ASSERT_EQ(2, (*iv_it).second);
    ++iv_it;
    ASSERT_EQ(4, (*iv_it).second);
    ++iv_it;
    ASSERT_EQ(3, (*iv_it).second);
    ++iv_it;
    ASSERT_EQ(1, (*iv_it).second);
}

TEST(Add, SameIdPresent) {
    SparseVector<int> c = SparseVector<int>(52);
    c.push_back(3, 5);
    c.push_back(11, 4);
    c.push_back(24, 3);

    SparseVector<int> d = SparseVector<int>(52);
    d.push_back(11, 2);
    d.push_back(30, 1);

    SparseVector<int> e = c + d;

    IVIterator<int> iv_it = e.iv_list.begin();
    ASSERT_EQ(5, (*iv_it).second);
    ++iv_it;
    ASSERT_EQ(6, (*iv_it).second);
    ++iv_it;
    ASSERT_EQ(3, (*iv_it).second);
    ++iv_it;
    ASSERT_EQ(1, (*iv_it).second);
}

TEST(GenMult, DisjointIdLists) {
    SparseVector<int> c = SparseVector<int>(52);
    c.push_back(3, 5);
    c.push_back(11, 4);
    c.push_back(24, 3);

    SparseVector<int> d = SparseVector<int>(52);
    d.push_back(8, 2);
    d.push_back(30, 1);

    SparseVector<int> e = c.GenMult(d, &Multiply);

    IVIterator<int> iv_it = e.iv_list.begin();
    ASSERT_EQ(e.iv_list.end(), iv_it);
}

TEST(Get, PresentElement) {
    SparseVector<int> c = SparseVector<int>(52);
    c.push_back(3, 5);
    c.push_back(11, 4);
    c.push_back(24, 3);

    ASSERT_EQ(5, *c.get(3));
    ASSERT_EQ(4, *c.get(11));
    ASSERT_EQ(3, *c.get(24));
}

TEST(Get, NonPresentElement) {
    SparseVector<int> c = SparseVector<int>(52);
    c.push_back(3, 5);
    c.push_back(11, 4);
    c.push_back(24, 3);

    // Failing right now - returns random memory.
    // ASSERT_EQ(NULL, *c.get(-1));
    // ASSERT_EQ(NULL, *c.get(2));
}

TEST(Reduce, Simple) {
    SparseVector<int> c = SparseVector<int>(52);
    c.push_back(3, 5);
    c.push_back(11, 4);
    c.push_back(24, 3);

    int result = c.Reduce(&Add, 0);
    ASSERT_EQ(12, result);
}

TEST(GenDot, Simple){
    SparseVector<int> c = SparseVector<int>(52);
    c.push_back(3, 5);
    c.push_back(11, 4);
    c.push_back(24, 3);

    SparseVector<int> d = SparseVector<int>(52);
    d.push_back(11, 2);
    d.push_back(30, 1);

    // Add-Mult Product
    int amresult = c.GenDot(d, &Multiply, &Add, 0);
    ASSERT_EQ(8, amresult);

    // Min-Plus semiring dot product
    int mpresult = c.GenDot(d,
                            [](int& a, int& b) { return a + b; },
                            [](int a, int b) { return min(a, b); },
                            INT32_MAX);
    ASSERT_EQ(6, mpresult);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}