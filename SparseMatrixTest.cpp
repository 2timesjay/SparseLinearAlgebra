#include <gtest/gtest.h>
#include "SparseVector.h"
#include "SparseMatrix.h"

using namespace std;

TEST(Construct, AddRows){
    SparseMatrix<int> m = SparseMatrix<int>(20, 30);
    SparseVector<int> a = SparseVector<int>(30);
    a.push_back(0, 4);
    a.push_back(20, 5);
    SparseVector<int> b = SparseVector<int>(30);
    b.push_back(15, 6);
    b.push_back(20, 7);
    m.add_row(1, a);
    m.add_row(18, b);

    ASSERT_EQ(4, *m.get(1, 0));
    ASSERT_EQ(5, *m.get(1, 20));
    ASSERT_EQ(6, *m.get(18, 15));
    ASSERT_EQ(7, *m.get(18, 20));
}

TEST(Construct, AddElements){
    SparseMatrix<int> m = SparseMatrix<int>(2, 3);
    m.add(0, 0, 4);
    m.add(0, 2, 5);
    m.add(1, 1, 6);
    m.add(1, 2, 7);

    ASSERT_EQ(4, *m.get(0, 0));
    ASSERT_EQ(5, *m.get(0, 2));
    ASSERT_EQ(6, *m.get(1, 1));
    ASSERT_EQ(7, *m.get(1, 2));
}

TEST(Transpose, Simple){
    SparseMatrix<int> m = SparseMatrix<int>(2, 3);
    m.add(0, 0, 4);
    m.add(0, 2, 5);
    m.add(1, 1, 6);
    m.add(1, 2, 7);

    SparseMatrix<int> mt = m.Transpose();

    ASSERT_EQ(4, *m.get(0, 0));
    ASSERT_EQ(5, *m.get(0, 2));
    ASSERT_EQ(6, *m.get(1, 1));
    ASSERT_EQ(7, *m.get(1, 2));
}

TEST(GenAddMat, Simple){
    SparseMatrix<int> m1 = SparseMatrix<int>(2, 3);
    m1.add(0, 0, 4);
    m1.add(0, 2, 5);
    m1.add(1, 1, 6);
    m1.add(1, 2, 7);

    SparseMatrix<int> m2 = SparseMatrix<int>(2, 3);
    m2.add(0, 0, 10);
    m2.add(0, 2, 10);
    m2.add(1, 0, 10);
    m2.add(1, 2, 10);

    SparseMatrix<int> m3 = m1.GenAddMat(m2, &Add<int>);

    ASSERT_EQ(14, *m3.get(0, 0));
    ASSERT_EQ(15, *m3.get(0, 2));
    ASSERT_EQ(10, *m3.get(1, 0));
    ASSERT_EQ(6, *m3.get(1, 1));
    ASSERT_EQ(17, *m3.get(1, 2));
}

TEST(GenMultMat, Simple){
    SparseMatrix<int> m1 = SparseMatrix<int>(2, 3);
    m1.add(0, 0, 4);
    m1.add(0, 2, 5);
    m1.add(1, 1, 6);
    m1.add(1, 2, 7);

    SparseMatrix<int> m2 = SparseMatrix<int>(2, 3);
    m2.add(0, 0, 10);
    m2.add(0, 2, 10);
    m2.add(1, 0, 10);
    m2.add(1, 2, 10);

    SparseMatrix<int> m3 = m1.GenMultMat(m2, [](int& a, int& b) { return a * b; });

    ASSERT_EQ(40, *m3.get(0, 0));
    ASSERT_EQ(50, *m3.get(0, 2));
    ASSERT_EQ(70, *m3.get(1, 2));
}


TEST(GenDotMat, Simple){
    SparseMatrix<int> m1 = SparseMatrix<int>(2, 3);
    m1.add(0, 0, 4);
    m1.add(0, 2, 5);
    m1.add(1, 1, 6);
    m1.add(1, 2, 7);

    SparseVector<int> v1 = SparseVector<int>(3);

    v1.push_back(0, 100);
    v1.push_back(1, 10);
    v1.push_back(2, 1);

    SparseVector<int> v2 = m1.GenDotMat(v1, &Multiply<int>, &Add<int>, 0);

    ASSERT_EQ(405, *v2.get(0));
    ASSERT_EQ(67, *v2.get(1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}