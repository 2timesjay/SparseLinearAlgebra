//
// Created by jacob on 3/26/17.
//

#ifndef CPPSPLA_ARRAYOPS_H
#define CPPSPLA_ARRAYOPS_H

#include <functional>
#include <iostream>
#include <list>

using namespace std;

template <class T>
using IdVal = pair<int, T>;

template <class T>
using IVIterator = typename list<IdVal<T>>::iterator;

template <class T>
using Reducer = function<T(T&, T&)>;

template <class T>
inline
T Add(T a, T b) {
    return a + b;
}

template <class T>
inline
T Multiply(T& a, T& b) {
    return a * b;
}

template <class T>
class SparseVector {
public:
    int size;
    list<IdVal<T>> iv_list;
    SparseVector<T> ();
    SparseVector<T> (int size);
    SparseVector<T> operator+(SparseVector other);
    SparseVector<T> operator*(SparseVector other);
    SparseVector<T> GenMult(SparseVector other, Reducer<T> mult);
    SparseVector<T> GenAdd(SparseVector other, Reducer<T> add);
    T Reduce(Reducer<T> reducef, T zero);
    T GenDot(SparseVector other, Reducer<T> elemf, Reducer<T> reducef, T zero);
    T* get(int id);
    bool has(int id);
    void print_string();
    void insert(int id, T val);
    void push_back(int id, T val);
};

template <class T>
inline
SparseVector<T>::SparseVector(){
    size = 0;
}


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
SparseVector<T> operator*(SparseVector<T>& a, SparseVector<T>& b) {
    return a * b;
}

template <class T>
inline
SparseVector<T> SparseVector<T>::operator+(SparseVector<T> other) {
    return GenAdd(other, &Add<T>);
}

template <class T>
inline
SparseVector<T> SparseVector<T>::operator*(SparseVector<T> other) {
    return GenMult(other, &Multiply);
}

/// Performs Generalized multiplication in the sense of a semiring.
/// Set of ids is an AND of the two. Where both are present, apply (*add).
template <class T>
inline
SparseVector<T> SparseVector<T>::GenMult(SparseVector<T> other, Reducer<T> mult) {
    list<IdVal<T>> merged_iv_list;
    IVIterator<T> iv_it = iv_list.begin();
    IVIterator<T> other_iv_it = other.iv_list.begin();
    int limiter = 5;
    while (iv_it != iv_list.end() and other_iv_it != other.iv_list.end() and limiter >= 0){
        --limiter;
        int id = (*iv_it).first;
        int other_id = (*other_iv_it).first;
        if (id < other_id){
            ++iv_it;
        }
        else if (id > other_id) {
            ++other_iv_it;
        }
        else if (id == other_id) {
            T val = (*iv_it).second;
            T other_val = (*other_iv_it).second;
            T merged_val = mult(val, other_val);
            merged_iv_list.push_back(IdVal<T>(id, merged_val));
            ++iv_it;
            ++other_iv_it;
        }
    }

    SparseVector<T> merged = SparseVector<T>(size + other.size);
    merged.iv_list = merged_iv_list;
    return merged;
}

/// Performs Generalized Addition in the sense of a semiring.
/// Set of ids is an OR of the two. Where both are present, apply (*add).
template <class T>
inline
SparseVector<T> SparseVector<T>::GenAdd(SparseVector<T> other, Reducer<T> add) {
    list<IdVal<T>> merged_iv_list;
    IVIterator<T> iv_it = iv_list.begin();
    IVIterator<T> other_iv_it = other.iv_list.begin();
    while (iv_it != iv_list.end() and other_iv_it != other.iv_list.end()) {
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
            T val = (*iv_it).second;
            T other_val = (*other_iv_it).second;
            merged_iv_list.push_back(IdVal<T>(id, add(val, other_val)));
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

    SparseVector<T> merged = SparseVector<T>(size + other.size);
    merged.iv_list = merged_iv_list;
    return merged;
}

template <class T>
inline
T SparseVector<T>::Reduce(Reducer<T> reducef, T zero) {
    T accum = zero;
    for (IVIterator<T> it=iv_list.begin(); it != iv_list.end(); ++it) {
        accum = reducef(accum, it->second);
    }
    return accum;
}

/// Performs Generalized Addition in the sense of a semiring.
/// Set of ids is an OR of the two. Where both are present, apply (*add).
template <class T>
inline
T SparseVector<T>::GenDot(SparseVector<T> other, Reducer<T> elemf, Reducer<T> reducef, T zero) {
    SparseVector<T> temp = this->GenMult(other, elemf);
    return temp.Reduce(reducef, zero);
}

template <class T>
inline
T* SparseVector<T>::get(int id) {
    for (IVIterator<T> it=iv_list.begin(); it != iv_list.end(); ++it) {
        if ((*it).first == id){
            return &(it->second);
        }
    }
}

template <class T>
inline
bool SparseVector<T>::has(int id) {
    for (IVIterator<T> it=iv_list.begin(); it != iv_list.end(); ++it) {
        if ((*it).first == id){
            return true;
        }
    }
    return false;
}

template <class T>
inline
void SparseVector<T>::print_string() {
    cout << "Printing SPV ";
    for (IVIterator<T> it=iv_list.begin(); it != iv_list.end(); ++it) {
        cout << (*it).first << " " << (*it).second << " ";
    }
    cout << endl;
}

template <class T>
inline
void SparseVector<T>::insert(int id, T val) {
    IVIterator<T> it = iv_list.begin();
    while(it->first < id && it != iv_list.end()){
        it++;
    }
    iv_list.insert(it, IdVal<T>(id, val));
}

template <class T>
inline
void SparseVector<T>::push_back(int id, T val) {
    int last_id = iv_list.end()->first;
    if (id < last_id){
        throw  "New id must be greater than any previous id.";
    }
    iv_list.push_back(IdVal<T>(id, val));
}

#endif //CPPSPLA_ARRAYOPS_H
