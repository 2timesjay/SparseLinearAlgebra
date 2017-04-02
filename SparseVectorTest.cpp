#include <gtest/gtest.h>
#include <list>
#include "SparseVector.h"

using namespace std;

TEST(Add, DisjointIdLists) {
    SparseVector<int> a(1);
    SparseVector<int> b = SparseVector<int>(33);
    SparseVector<int> summed_spv = a + b;

    SparseVector<int> c = SparseVector<int>(52);
    c.push_back(3, 5);
    c.push_back(11, 4);
    c.push_back(24, 3);

    SparseVector<int> d = SparseVector<int>(52);
    d.push_back(8, 2);
    d.push_back(30, 1);

    SparseVector<int> e = c + d;

    list<int>::iterator val_it=e.val_list.begin();
    ASSERT_EQ(5, *val_it);
    ++val_it;
    ASSERT_EQ(2, *val_it);
    ++val_it;
    ASSERT_EQ(4, *val_it);
    ++val_it;
    ASSERT_EQ(3, *val_it);
    ++val_it;
    ASSERT_EQ(1, *val_it);
}

TEST(Add, SameIdPresent) {
    SparseVector<int> a(1);
    SparseVector<int> b = SparseVector<int>(33);
    SparseVector<int> summed_spv = a + b;

    SparseVector<int> c = SparseVector<int>(52);
    c.push_back(3, 5);
    c.push_back(11, 4);
    c.push_back(24, 3);

    SparseVector<int> d = SparseVector<int>(52);
    d.push_back(11, 2);
    d.push_back(30, 1);

    SparseVector<int> e = c + d;

    list<int>::iterator val_it=e.val_list.begin();
    ASSERT_EQ(5, *val_it);
    ++val_it;
    ASSERT_EQ(6, *val_it);
    ++val_it;
    ASSERT_EQ(3, *val_it);
    ++val_it;
    ASSERT_EQ(1, *val_it);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}