#include <iostream>
#include <list>
#include <algorithm>
#include "SparseVector.h"

using namespace std;

int main() {
    SparseVector<int> a(1);
    SparseVector<int> b = SparseVector<int>(33);
    SparseVector<int> summed_spv = a + b;
    cout << summed_spv.size << endl;

    SparseVector<int> c = SparseVector<int>(52);
    c.push_back(3, 533);
    c.push_back(11, 51111);
    c.push_back(24, 52424);

    SparseVector<int> d = SparseVector<int>(52);
    d.push_back(8, 588);
    d.push_back(30, 53030);

    cout << "size d " << d.id_list.size() << endl;

    SparseVector<int> e = c + d;

    cout << "size e " << e.size << endl;
    cout << "size e " << e.id_list.size() << endl;

    for (list<int>::iterator it=e.id_list.begin(); it != e.id_list.end(); ++it) {
        cout << *it << endl;
    }

    for (list<int>::iterator it=e.val_list.begin(); it != e.val_list.end(); ++it) {
        cout << *it << endl;
    }

    return 0;
}