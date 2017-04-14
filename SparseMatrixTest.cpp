#include <gtest/gtest.h>
#include "SparseVector.h"
#include "SparseMatrix.h"

using namespace std;

//TEST(Construct, AddRows){
//    SparseMatrix<int> m = SparseMatrix<int>(20, 30);
//    SparseVector<int> a = SparseVector<int>(30);
//    a.push_back(0, 4);
//    a.push_back(20, 5);
//    SparseVector<int> b = SparseVector<int>(30);
//    b.push_back(15, 6);
//    b.push_back(20, 7);
//    m.add_row(1, a);
//    m.add_row(18, b);
//
//    ASSERT_EQ(4, m.get(1, 0));
//    ASSERT_EQ(5, m.get(1, 20));
//    ASSERT_EQ(6, m.get(18, 15));
//    ASSERT_EQ(7, m.get(18, 20));
//}

TEST(Construct, AddElements){
    SparseMatrix<int> m = SparseMatrix<int>(2, 3);
    m.add(0, 0, 4);
    m.add(0, 2, 5);
    m.add(1, 1, 6);
    m.add(1, 2, 7);

    ASSERT_EQ(4, m.get(0, 0));
    ASSERT_EQ(5, m.get(0, 2));
    ASSERT_EQ(6, m.get(1, 1));
    ASSERT_EQ(7, m.get(1, 2));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}