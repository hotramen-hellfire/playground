#include <bits/stdc++.h>
#include "rbt.h"
using namespace std;
// Main function for testing
int main() {
    RedBlackTree rbt;

    // Inserting values
    cout << "Inserting values:" << endl;
    vector<pair<float, int>> insertions = {
        {5.6f, 10}, {3.2f, 20}, {7.8f, 30}, {5.6f, 40},
        {2.5f, 50}, {4.7f, 60}, {3.1f, 70}, {7.2f, 80},
        {6.3f, 90}, {8.4f, 100}, {1.1f, 110}, {9.5f, 120}
    };
    for (auto &p : insertions) {
        rbt.insert(p.first, p.second);
    }
    rbt.printRBT(rbt.root);

    // Searching for a value
    cout << "\nSearching for value with key 5.6:" << endl;
    Node *searchResult = rbt.search(5.6f);
    if (searchResult != nullptr) {
        cout << "Found node with key 5.6:" << endl;
        for (int value : searchResult->values) {
            cout << "Value: " << value << endl;
        }
    } else {
        cout << "Node with key 5.6 not found" << endl;
    }

    // Finding predecessor and successor of a key
    cout << "\nFinding predecessor and successor of key 5.6:" << endl;
    Node *pred = rbt.predecessor(5.6f);
    if (pred != nullptr) {
        cout << "Predecessor of key 5.6: " << pred->key << endl;
    } else {
        cout << "Predecessor not found" << endl;
    }

    Node *succ = rbt.successor(5.6f);
    if (succ != nullptr) {
        cout << "Successor of key 5.6: " << succ->key << endl;
    } else {
        cout << "Successor not found" << endl;
    }

    // Deleting a value
    // cout << "\nDeleting value 40 with key 5.6:" << endl;
    cout << "\nDeleting all values:" << endl;
    for (auto &p : insertions) {
        rbt.erase(p.first, p.second);
        rbt.printRBT(rbt.root);
    }

    // Clearing the Red-Black Tree
    // cout << "\nClearing Red-Black Tree:" << endl;
    // rbt.clearRBT(rbt.root);
    // rbt.printRBT(rbt.root);

    return 0;
}



