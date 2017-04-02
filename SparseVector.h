//
// Created by jacob on 3/26/17.
//

#ifndef CPPSPEEDY_ARRAYOPS_H
#define CPPSPEEDY_ARRAYOPS_H

#include <iostream>
#include <list>

using namespace std;

template <class T>
using IdValue = pair<int, T>;

template <class T>
class SparseVector {
public:
    int size;
    list<int> id_list;
    list<T> val_list;
    SparseVector<T> (int size);
    SparseVector<T> operator+(SparseVector spv);
    void push_back(int id, T val);
};

template <class T>
inline
SparseVector<T>::SparseVector(int input_size){
    size = input_size;
}

template <class T>
inline
SparseVector<T> SparseVector<T>::operator+(SparseVector<T> other) {
    list<int> merged_id_list;
    list<T> merged_val_list;

    list<int> other_id_list = other.id_list;
    list<int>::iterator other_it = other_id_list.begin();
    list<int>::iterator val_it = val_list.begin();
    list<int>::iterator other_val_it = other.val_list.begin();
    for (list<int>::iterator it=id_list.begin(); it != id_list.end(); ++it) {
        while (*other_it < *it) {
            merged_id_list.push_back(*other_it);
            ++other_it;
            merged_val_list.push_back(*other_val_it);
            ++other_val_it;
        }
        if (*other_it == *it) {
            merged_id_list.push_back(*other_it);
            ++other_it;
            merged_val_list.push_back(*val_it + *other_val_it);
            ++val_it;
            ++other_val_it;
        }
        else {
            merged_id_list.push_back(*it);
            merged_val_list.push_back(*val_it);
            ++val_it;
        }
    }
    while (other_it != other_id_list.end()) {
        merged_id_list.push_back(*other_it);
        ++other_it;
        merged_val_list.push_back(*other_val_it);
        ++other_val_it;
    }

    SparseVector<T> merged = SparseVector<T>(size + other.size);
    list<int>::iterator merged_val_it=merged_val_list.begin();
    for (list<int>::iterator merged_it=merged_id_list.begin(); merged_it != merged_id_list.end(); ++merged_it) {
        merged.push_back(*merged_it, *merged_val_it);
        ++merged_val_it;
    }
    return merged;
}

template <class T>
inline
void SparseVector<T>::push_back(int id, T val) {
    id_list.push_back(id);
    val_list.push_back(val);
}

#endif //CPPSPEEDY_ARRAYOPS_H
