/*
 * Sparse Matrix file includes sparse vector and sparse matrix.
 * This is simple practice to see if I can make relatively optimized sparse vectors and matrices.
 *
 */

#ifndef CPPSPLA_MATOPS_H
#define CPPSPLA_MATOPS_H

using namespace std;

template <class T>
class SparseMatrix {
public:
    int r;
    int c;
    SparseVector<SparseVector<T>> row_list;

    SparseMatrix<T> (int r, int c);
};

#endif //CPPSPLA_MATOPS_H