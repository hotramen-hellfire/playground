#include <bits/stdc++.h>
#include "bst.h"
using namespace std;
// Main function for testing
int main() {
    BinarySearchTree bst;

    // Inserting values
    cout << "Inserting values:" << endl;
    vector<pair<float, int>> insertions = {
        {49, 0}, {15.7692, 1}, {39, 2}
    };
    for (auto &p : insertions) {
        bst.insert(p.first, p.second);
    }
    bst.printBST(bst.root);
    Node* pred = bst.successor(39);
    cout<<pred->key<<endl;
    return 0;
}



