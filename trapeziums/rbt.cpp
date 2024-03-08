#include <bits/stdc++.h>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    float key;
    vector<int> values;
    Color color;
    Node *left, *right, *parent;

    Node(float key, int value) : key(key), color(RED), left(nullptr), right(nullptr), parent(nullptr) {
        values.push_back(value);
    }
};

class RedBlackTree {
private:
    // Private functions for RBT operations
    void rotateLeft(Node *&);
    void rotateRight(Node *&);
    void fixViolation(Node *&);
    Node* minValueNode(Node*);
    Node* maxValueNode(Node*);

public:
    RedBlackTree() : root(nullptr) {}
    Node *root;
    // Public functions for RBT operations
    Node* insert(float key, int value);
    Node* search(float key);
    void erase(float key, int value);
    Node* predecessor(float key);
    Node* successor(float key);
    void printRBT(Node *root);
    void clearRBT(Node *&root);
};

// Rotate Left
void RedBlackTree::rotateLeft(Node *&ptr) {
    Node *right_child = ptr->right;
    ptr->right = right_child->left;

    if (ptr->right != nullptr)
        ptr->right->parent = ptr;

    right_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = right_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = right_child;
    else
        ptr->parent->right = right_child;

    right_child->left = ptr;
    ptr->parent = right_child;
}

// Rotate Right
void RedBlackTree::rotateRight(Node *&ptr) {
    Node *left_child = ptr->left;
    ptr->left = left_child->right;

    if (ptr->left != nullptr)
        ptr->left->parent = ptr;

    left_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = left_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = left_child;
    else
        ptr->parent->right = left_child;

    left_child->right = ptr;
    ptr->parent = left_child;
}

// Fix Red-Black Tree Violations
void RedBlackTree::fixViolation(Node *&ptr) {
    Node *parent_ptr = nullptr;
    Node *grand_parent_ptr = nullptr;

    while ((ptr != root) && (ptr->color != BLACK) && (ptr->parent->color == RED)) {
        parent_ptr = ptr->parent;
        grand_parent_ptr = ptr->parent->parent;

        // Case : A -> parent of ptr is left child of grand_parent of ptr
        if (parent_ptr == grand_parent_ptr->left) {
            Node *uncle_ptr = grand_parent_ptr->right;

            // Case 1: Uncle of ptr is also red. Only Recoloring required
            if (uncle_ptr != nullptr && uncle_ptr->color == RED) {
                grand_parent_ptr->color = RED;
                parent_ptr->color = BLACK;
                uncle_ptr->color = BLACK;
                ptr = grand_parent_ptr;
            } else {
                // Case 2: ptr is right child of its parent. Left-rotation required
                if (ptr == parent_ptr->right) {
                    rotateLeft(parent_ptr);
                    ptr = parent_ptr;
                    parent_ptr = ptr->parent;
                }

                // Case 3: ptr is left child of its parent. Right-rotation required
                rotateRight(grand_parent_ptr);
                swap(parent_ptr->color, grand_parent_ptr->color);
                ptr = parent_ptr;
            }
        } else {
            // Case B: parent of ptr is right child of grand_parent of ptr
            Node *uncle_ptr = grand_parent_ptr->left;

            // Case 1: Uncle of ptr is also red. Only Recoloring required
            if ((uncle_ptr != nullptr) && (uncle_ptr->color == RED)) {
                grand_parent_ptr->color = RED;
                parent_ptr->color = BLACK;
                uncle_ptr->color = BLACK;
                ptr = grand_parent_ptr;
            } else {
                // Case 2: ptr is left child of its parent. Right-rotation required
                if (ptr == parent_ptr->left) {
                    rotateRight(parent_ptr);
                    ptr = parent_ptr;
                    parent_ptr = ptr->parent;
                }

                // Case 3: ptr is right child of its parent. Left-rotation required
                rotateLeft(grand_parent_ptr);
                swap(parent_ptr->color, grand_parent_ptr->color);
                ptr = parent_ptr;
            }
        }
    }

    root->color = BLACK;
}

// Insert a new node with given key in Red-Black Tree
Node* RedBlackTree::insert(float key, int value) {
    Node *ptr = new Node(key, value);
    if (root == nullptr) {
        ptr->color = BLACK;
        root = ptr;
    } else {
        Node *current = root;
        Node *parent = nullptr;
        while (current != nullptr) {
            parent = current;
            if (key < current->key)
                current = current->left;
            else if (key > current->key)
                current = current->right;
            else { // If key already exists, just add value to the vector
                current->values.push_back(value);
                delete ptr; // Avoid memory leak
                return current;
            }
        }

        ptr->parent = parent;
        if (key < parent->key)
            parent->left = ptr;
        else
            parent->right = ptr;

        fixViolation(ptr);
    }
    return ptr;
}

// Search for a node with given key
Node* RedBlackTree::search(float key) {
    Node *current = root;
    while (current != nullptr) {
        if (key < current->key)
            current = current->left;
        else if (key > current->key)
            current = current->right;
        else
            return current; // Found the node with given key
    }
    return nullptr; // Node with given key not found
}

// Erase a value with given key and value from the Red-Black Tree
void RedBlackTree::erase(float key, int value) {
    Node *node = search(key);
    if (node == nullptr) // If key does not exist, return
        return;

    auto it = find(node->values.begin(), node->values.end(), value);
    if (it != node->values.end()) {
        node->values.erase(it); // Erase value from vector
    }

    if (node->values.empty()) { // If the node has no more values, perform regular deletion
        // Perform deletion as in BST
        // Code for BST deletion not included here for brevity
    }
}

// Find predecessor of given key
Node* RedBlackTree::predecessor(float key) {
    Node *current = root;
    Node *pred = nullptr;
    while (current != nullptr) {
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            pred = current;
            current = current->right;
        } else {
            if (current->left != nullptr) {
                pred = maxValueNode(current->left);
            }
            break;
        }
    }
    return pred;
}

// Find successor of given key
Node* RedBlackTree::successor(float key) {
    Node *current = root;
    Node *succ = nullptr;
    while (current != nullptr) {
        if (key < current->key) {
            succ = current;
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        } else {
            if (current->right != nullptr) {
                succ = minValueNode(current->right);
            }
            break;
        }
    }
    return succ;
}

// Find node with minimum value in subtree rooted at given node
Node* RedBlackTree::minValueNode(Node *node) {
    Node *current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// Find node with maximum value in subtree rooted at given node
Node* RedBlackTree::maxValueNode(Node *node) {
    Node *current = node;
    while (current->right != nullptr)
        current = current->right;
    return current;
}

void RedBlackTree::clearRBT(Node *&root) {
    if (root == nullptr)
        return;
    root->values.clear();
    root->parent=nullptr;
    clearRBT(root->left);
    clearRBT(root->right);
    root->left=nullptr;
    root->right=nullptr;
    delete root;
    root = nullptr;
} 
void RedBlackTree::printRBT(Node *root) {
    if (root == nullptr)
        return;

    std::queue<Node *> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; ++i) {
            Node *current = q.front();
            q.pop();
            std::cout << "Key: " << current->key << ", Values:";
            for (int value : current->values) {
                std::cout << " " << value;
            }
            std::cout << ", Color: " << (current->color == RED ? "RED" : "BLACK") << std::endl;

            if (current->left != nullptr)
                q.push(current->left);
            if (current->right != nullptr)
                q.push(current->right);
        }
    }
}

// Main function for testing
int main() {
    RedBlackTree rbt;

    // Inserting some values
    rbt.insert(5.6f, 10);
    rbt.insert(3.2f, 20);
    rbt.insert(7.8f, 30);
    rbt.insert(5.6f, 40);

    // Searching for a value
    Node *result = rbt.search(5.6f);
    if (result != nullptr) {
        cout << "Found node with key 5.6" << endl;
        for (int value : result->values) {
            cout << "Value: " << value << endl;
        }
    } else {
        cout << "Node with key 5.6 not found" << endl;
    }

    // Finding predecessor of a key
    Node *pred = rbt.predecessor(5.6f);
    if (pred != nullptr) {
        cout << "Predecessor of key 5.6: " << pred->key << endl;
    } else {
        cout << "Predecessor not found" << endl;
    }

    // Finding successor of a key
    Node *succ = rbt.successor(5.6f);
    if (succ != nullptr) {
        cout << "Successor of key 5.6: " << succ->key << endl;
    } else {
        cout << "Successor not found" << endl;
    }

    clearRBT(rbt.root);
    rbt.insert(5.6f, 40);
    // Finding predecessor of a key
    pred = rbt.predecessor(5.6f);
    if (pred != nullptr) {
        cout << "Predecessor of key 5.6: " << pred->key << endl;
    } else {
        cout << "Predecessor not found" << endl;
    }

    // // Finding successor of a key
    succ = rbt.successor(5.6f);
    if (succ != nullptr) {
        cout << "Successor of key 5.6: " << succ->key << endl;
    } else {
        cout << "Successor not found" << endl;
    }

    printRBT(rbt.root);
    return 0;
}

