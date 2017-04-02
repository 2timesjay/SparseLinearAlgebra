//////
////// Created by jacob on 3/26/17.
//////
//
//#include "SparseVector.h"
//
//#include <list>
//
//using namespace std;
//
//template <class T>
//SparseVector<T> SparseVector<T>::operator+(SparseVector<T> spv) {
//    list<int> merged_id_list;
//    list<int> other_id_list = spv.id_list;
//    list<int>::iterator other_it = other_id_list.begin();
//    for (list<int>::iterator it=id_list.begin(); it != id_list.end(); ++it) {
//        while (*other_it <= *it) {
//            merged_id_list.push_back(*other_it);
//            ++other_it;
//        }
//        merged_id_list.push_back(*other_it);
//    }
//    while (other_it != other_id_list.end()) {
//        merged_id_list.push_back(*other_it);
//        ++other_it;
//    }
//
//    return SparseVector<T>(size + spv.size);
//}
//
//template SparseVector<int> SparseVector<int>::operator+(SparseVector<int> spv);
//
//template <class T>
//SparseVector<T> SparseVector<T>::push_back(int id, T val) {
//    id_list.push_back(id);
//}
//
//template SparseVector<int> SparseVector<int>::push_back(int id, int val);