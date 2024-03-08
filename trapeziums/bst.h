#include <iostream>
#include <vector>
#include <algorithm>
#define EPSILON 0.000001
struct Node {
    float key;
    std::vector<int> values;
    Node *left, *right;

    Node(float key, int value) : key(key), left(nullptr), right(nullptr) {
        values.push_back(value);
    }
};

class BinarySearchTree {
public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { clearBST(root); }
    Node* root;

    Node* insert(Node* node, float key, int value);
    Node* search(Node* node, float key);
    Node* minValueNode(Node* node);
    Node* maxValueNode(Node* node);
    Node* deleteNode(Node* root, float key, int value);
    void insert(float key, int value);
    Node* search(float key);
    void erase(float key, int value);
    Node* predecessor(float key);
    Node* successor(float key);
    void printBST(Node* root);
    void clearBST(Node* &root);
};

void BinarySearchTree::insert(float key, int value) {
    root = insert(root, key, value);
}

Node* BinarySearchTree::insert(Node* node, float key, int value) {
    if (node == nullptr)
        return new Node(key, value);

    if (key < node->key)
        node->left = insert(node->left, key, value);
    else if (key > node->key)
        node->right = insert(node->right, key, value);
    else
        node->values.push_back(value);

    return node;
}

Node* BinarySearchTree::search(float key) {
    return search(root, key);
}

Node* BinarySearchTree::search(Node* node, float key) {
    if (node == nullptr || node->key == key)
        return node;

    if (key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}

void BinarySearchTree::erase(float key, int value) {
    root = deleteNode(root, key, value);
}

Node* BinarySearchTree::deleteNode(Node* node, float key, int value) {
    if (node == nullptr)
        return node;

    if (key < node->key)
        node->left = deleteNode(node->left, key, value);
    else if (key > node->key)
        node->right = deleteNode(node->right, key, value);
    else {
        auto it = std::find(node->values.begin(), node->values.end(), value);
        if (it != node->values.end())
            node->values.erase(it);

        if (node->values.empty()) {
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* temp = minValueNode(node->right);
            node->key = temp->key;
            node->values = temp->values;
            node->right = deleteNode(node->right, temp->key, temp->values[0]);
        }
    }
    return node;
}

Node* BinarySearchTree::minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

Node* BinarySearchTree::maxValueNode(Node* node) {
    Node* current = node;
    while (current->right != nullptr)
        current = current->right;
    return current;
}


Node* BinarySearchTree::predecessor(float key) {
    Node* current = root;
    Node* pred = nullptr;
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

Node* BinarySearchTree::successor(float key) {
    Node* current = root;
    Node* succ = nullptr;
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

void BinarySearchTree::printBST(Node* root) {
    if (root == nullptr)
        return;

    std::cout << "Key: " << root->key << ", Values:";
    for (int value : root->values) {
        std::cout << " " << value;
    }
    std::cout << std::endl;

    printBST(root->left);
    printBST(root->right);
}

void BinarySearchTree::clearBST(Node *&root) {
    if (root == nullptr)
        return;
    root->values.clear();
    // root->parent=nullptr;
    clearBST(root->left);
    clearBST(root->right);
    root->left=nullptr;
    root->right=nullptr;
    delete root;
    root = nullptr;
} 
