//
// Created by jacob on 3/26/17.
//

#ifndef CPPSPLA_ARRAYOPS_H
#define CPPSPLA_ARRAYOPS_H

#include <iostream>
#include <list>

using namespace std;

template <class T>
using IdVal = pair<int, T>;

template <class T>
using IVIterator = typename list<IdVal<T>>::iterator;

template <class T>
class SparseVector {
public:
    int size;
    list<IdVal<T>> iv_list;
    SparseVector<T> (int size);
    SparseVector<T> operator+(SparseVector other);
    SparseVector<T> operator&&(SparseVector other);
    void push_back(int id, T val);
};

template <class T>
inline
SparseVector<T>::SparseVector(int input_size){
    size = input_size;
}

template <class T>
inline
SparseVector<T> operator+(SparseVector<T> a, SparseVector<T> b) {
    return a + b;
}

template <class T>
inline
SparseVector<T> SparseVector<T>::operator+(SparseVector<T> other) {
    list<IdVal<T>> merged_iv_list;
    IVIterator<T> iv_it = iv_list.begin();
    IVIterator<T> other_iv_it = other.iv_list.begin();
    int limiter = 0;
    while (iv_it != iv_list.end() and other_iv_it != other.iv_list.end() and limiter < 100){
        ++limiter;
        int id = (*iv_it).first;
        int other_id = (*other_iv_it).first;
        if (id < other_id){
            merged_iv_list.push_back(*iv_it);
            ++iv_it;
        }
        else if (id > other_id) {
            merged_iv_list.push_back(*other_iv_it);
            ++other_iv_it;
        }
        else if (id == other_id) {
            int val = (*iv_it).second;
            int other_val = (*other_iv_it).second;
            merged_iv_list.push_back(IdVal<T>(id, val + other_val));
            ++iv_it;
            ++other_iv_it;
        }
    }
    //Ensure that the longer iv_list has its remaining elements added.
    while (iv_it != iv_list.end()) {
        merged_iv_list.push_back(*iv_it);
        ++iv_it;
    }
    while (other_iv_it != other.iv_list.end()) {
        merged_iv_list.push_back(*other_iv_it);
        ++other_iv_it;
    }

//    for (IVIterator<T> merged_it=merged_iv_list.begin(); merged_it != merged_iv_list.end(); ++merged_it) {
//        cout << (*merged_it).first << " " << (*merged_it).second << endl;
//    }

    SparseVector<T> merged = SparseVector<T>(size + other.size);
    merged.iv_list = merged_iv_list;
    return merged;
}

/// Should Serve as basis for other operations: && type ops and || type ops with reductions on elements.
/// \tparam T
/// \param other
/// \return
template <class T>
inline
SparseVector<T> SparseVector<T>::operator&&(SparseVector<T> other) {
    list<IdVal<T>> merged_iv_list;
    IVIterator<T> iv_it = iv_list.begin();
    IVIterator<T> other_iv_it = other.iv_list.begin();
    int limiter = 0;
    while (iv_it != iv_list.end() and other_iv_it != other.iv_list.end() and limiter < 100){
        ++limiter;
        int id = (*iv_it).first;
        int other_id = (*other_iv_it).first;
        if (id < other_id){
            ++iv_it;
        }
        else if (id > other_id) {
            ++other_iv_it;
        }
        else if (id == other_id) {
            int val = (*iv_it).second;
            int other_val = (*other_iv_it).second;
            merged_iv_list.push_back(IdVal<T>(id, val + other_val));
            ++iv_it;
            ++other_iv_it;
        }
    }

    SparseVector<T> merged = SparseVector<T>(size + other.size);
    merged.iv_list = merged_iv_list;
    return merged;
}

template <class T>
inline
void SparseVector<T>::push_back(int id, T val) {
    iv_list.push_back(IdVal<T>(id, val));
}

#endif //CPPSPLA_ARRAYOPS_H
