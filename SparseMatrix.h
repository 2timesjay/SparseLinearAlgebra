/*
 * Sparse Matrix file includes sparse vector and sparse matrix.
 * This is simple practice to see if I can make relatively optimized sparse vectors and matrices.
 *
 */

#ifndef CPPSPLA_MATOPS_H
#define CPPSPLA_MATOPS_H

using namespace std;

#include <iostream>
#include <list>
#include "SparseVector.h"

template <class T>
class SparseMatrix {
public:
    int rows;
    int cols;
    int size;
    SparseVector<SparseVector<T>> row_list;

    SparseMatrix<T> ();
    SparseMatrix<T> (int r, int c);
    T get(int r, int c);
    void add(int r, int c, T val);
    void add_row(int r, SparseVector<T> v);
    SparseMatrix<T> Transpose();
};

template <class T>
inline
SparseMatrix<T>::SparseMatrix(){
    rows = 0;
    cols = 0;
    size = 0;
    row_list = SparseVector<SparseVector<T>>(rows);
}

template <class T>
inline
SparseMatrix<T>::SparseMatrix(int r, int c){
    rows = r;
    cols = c;
    size = 0;
    row_list = SparseVector<SparseVector<T>>(rows);
}

template <class T>
inline
SparseMatrix<T> SparseMatrix<T>::Transpose() {
    SparseMatrix<T> mt = SparseMatrix<T>(cols, rows);
    return mt;
}

template <class T>
inline
void SparseMatrix<T>::add(int r, int c, T val){
    // Assumes adding to end, even though that's not enforced. Id can occur multiple times or be disordered.
    SparseVector<T> *row;
    if (!row_list.has(r)) {
        row = new SparseVector<T>(cols);
        add_row(r, *row);
    }
    row = row_list.get(r);
    ++size;
    row->push_back(c, val);
//    cout << r << " " << c << " " << val << endl;
//    row->print_string();
//    row_list.get(r)->print_string();
//    cout << " --- " << endl;
}

template <class T>
inline
T SparseMatrix<T>::get(int r, int c){
    SparseVector<T> *row = row_list.get(r);
    T val = *(row->get(c));
    return val;
}

template <class T>
inline
void SparseMatrix<T>::add_row(int r, SparseVector<T> v){
    row_list.push_back(r, v);
}


#endif //CPPSPLA_MATOPS_H