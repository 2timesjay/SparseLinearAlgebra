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
    T* get(int r, int c);
    void add(int r, int c, T val);
    void add_row(int r, SparseVector<T> v);
    SparseMatrix<T> Transpose();
    SparseMatrix<T> GenAddMat(SparseMatrix<T> other, SparseVector<T> (*add)(SparseVector<T>, SparseVector<T>));
    SparseMatrix<T> GenMultMat(SparseMatrix<T> other, T (*mult)(T, T));
};

template <class T>
inline
SparseVector<T> AddElements(SparseVector<T> a, SparseVector<T> b) {
    return a.GenAdd(b, &Add);
}

template <class T>
inline
SparseVector<T> MultiplyElements(SparseVector<T> a, SparseVector<T> b) {
    return a.GenMult(b, &Multiply);
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
SparseMatrix<T> SparseMatrix<T>::GenAddMat(SparseMatrix<T> other, SparseVector<T> (*add)(SparseVector<T>, SparseVector<T>)){
    SparseMatrix<T> mt = SparseMatrix<T>(rows, cols);
    mt.row_list = row_list.GenAdd(other.row_list, *add);
    return mt;
}

template <class T>
inline
SparseMatrix<T> SparseMatrix<T>::GenMultMat(SparseMatrix<T> other, T (*mult)(T, T)){
    cout << "Elem Mult: " << endl;
    SparseMatrix<T> mt = SparseMatrix<T>(rows, cols);
//    mt.row_list = row_list.GenMult(other.row_list,
//                                   [](SparseVector<T> a, SparseVector<T> b)->SparseVector<T>{
//                                       cout << "Sparse Vector Lambda" << endl;
//                                       cout << a.size << endl;
//                                       cout << b.size << endl;
//                                       cout << *a.get(0) * *b.get(0) << endl;
//                                       cout << "Sparse Vector Lambda" << endl;
//                                       SparseVector<T> c = MultiplyElements(a, b);
//                                       cout << "Sparse Vector Lambda" << endl;
//                                       cout << *c.get(0) << endl;
//                                       cout << "Sparse Vector Lambda" << endl;
//                                       return a.GenMult(b, &Multiply);
//                                   });

//    auto fptr = [](SparseVector<T> a, SparseVector<T> b) {
//        cout << "Sparse Vector Lambda" << endl;
//        return a.GenMult(b, [](T ea, T eb) {return ea * eb;} );
//    };
//    mt.row_list = row_list.GenMult(other.row_list,
//                                   fptr);

    mt.row_list = row_list.GenMult(other.row_list,
                                   &MultiplyElements);
    return mt;
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