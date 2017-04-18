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
#include <functional>
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
    T* get(int r, int c);
    void add(int r, int c, T val);
    void add_row(int r, SparseVector<T> v);
    SparseMatrix<T> Transpose();
    SparseMatrix<T> GenAddMat(SparseMatrix<T> other, Reducer<T> add);
    SparseMatrix<T> GenMultMat(SparseMatrix<T> other, Reducer<T> mult);
    SparseVector<T> GenDotMat(SparseVector<T> other, Reducer<T> elemf, Reducer<T> reducef, T zero);
};

template <class T>
inline
SparseVector<T> AddElements(SparseVector<T> a, SparseVector<T> b) {
    return a.GenAdd(b, &Add);
}

template <class T>
inline
SparseVector<T> GenAddElements(T (*add)(T, T), SparseVector<T> a, SparseVector<T> b) {
    return a.GenAdd(b, add);
}

template <class T>
inline
SparseVector<T> MultiplyElements(SparseVector<T>& a, SparseVector<T>& b) {
    return a.GenMult(b, &Multiply);
}

template <class T>
inline
SparseVector<T> GenMultiplyElements(T (*mult)(T&, T&), SparseVector<T> a, SparseVector<T> b) {
    return a.GenMult(b, mult);
}

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
    for (IVIterator<SparseVector<T>> row_it=row_list.iv_list.begin(); row_it != row_list.iv_list.end(); ++row_it) {
        int r = row_it->first;
        SparseVector<T> row = row_it->second;
        for (IVIterator<T> it=row.iv_list.begin(); it != row.iv_list.end(); ++it) {
            int c = it->first;
            T val = it->second;
            mt.add(c, r, val);
        }
    }
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
}

// TODO: Rewrite to use just T, T -> T lambdas and intelligently create the lambda for vector addition/multiplication.
template <class T>
inline
SparseMatrix<T> SparseMatrix<T>::GenAddMat(SparseMatrix<T> other, Reducer<T> add){
    SparseMatrix<T> mt = SparseMatrix<T>(rows, cols);

    Reducer<SparseVector<T>> add_vectorized = [add](SparseVector<T>& a, SparseVector<T>&b)->SparseVector<T> {
        return a.GenAdd(b, add);
    };
    mt.row_list = row_list.GenAdd(other.row_list, add_vectorized);
    return mt;
}

template <class T>
inline
SparseMatrix<T> SparseMatrix<T>::GenMultMat(SparseMatrix<T> other, Reducer<T> mult){
    SparseMatrix<T> mt = SparseMatrix<T>(rows, cols);

    Reducer<SparseVector<T>> mult_vectorized = [mult](SparseVector<T>& a, SparseVector<T>&b)->SparseVector<T> {
        return a.GenMult(b, mult);
    };
    mt.row_list = row_list.GenMult(other.row_list, mult_vectorized);
    return mt;
}

//// TODO: Rewrite to use just T, T -> T lambdas and intelligently create the lambda for vector addition/multiplication.
template <class T>
inline
SparseVector<T> SparseMatrix<T>::GenDotMat(SparseVector<T> other, Reducer<T> elemf, Reducer<T> reducef, T zero){
    SparseVector<T> result = SparseVector<T>(rows);
    for (IVIterator<SparseVector<T>> row_it=row_list.iv_list.begin(); row_it != row_list.iv_list.end(); ++row_it) {
        int r = row_it->first;
        SparseVector<T> row = row_it->second;
        result.push_back(r, row.GenDot(other, elemf, reducef, zero));
    }
    return result;
}

template <class T>
inline
T* SparseMatrix<T>::get(int r, int c){
    SparseVector<T> *row = row_list.get(r);
    return row->get(c);
}

template <class T>
inline
void SparseMatrix<T>::add_row(int r, SparseVector<T> v){
    row_list.push_back(r, v);
}


#endif //CPPSPLA_MATOPS_H