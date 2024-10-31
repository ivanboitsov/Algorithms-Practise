#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <climits>
#include <cstdint>
#include <chrono>

using namespace std;

struct Node {
    string key;
    uint64_t value;
    int priority;
    Node* left;
    Node* right;

    Node(const string& k, uint64_t v)
        : key(k), value(v), priority(rand()), left(nullptr), right(nullptr) {}
};

class Treap {
private:
    Node* root;

    void split(Node* current, const string& key, Node*& left, Node*& right) {
        if (!current) {
            left = right = nullptr;
        }
        else if (current->key < key) {
            split(current->right, key, current->right, right);
            left = current;
        }
        else {
            split(current->left, key, left, current->left);
            right = current;
        }
    }

    Node* merge(Node* left, Node* right) {
        if (!left || !right) return left ? left : right;
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            return left;
        }
        else {
            right->left = merge(left, right->left);
            return right;
        }
    }

    bool insert(Node*& current, const string& key, uint64_t value) {
        if (!current) {
            current = new Node(key, value);
            return true;
        }
        if (current->key == key) return false;
        if (current->key > key) {
            if (insert(current->left, key, value)) {
                if (current->left->priority > current->priority)
                    current = rotateRight(current);
                return true;
            }
        }
        else {
            if (insert(current->right, key, value)) {
                if (current->right->priority > current->priority)
                    current = rotateLeft(current);
                return true;
            }
        }
        return false;
    }

    bool erase(Node*& current, const string& key) {
        if (!current) return false;
        if (current->key == key) {
            Node* old = current;
            current = merge(current->left, current->right);
            delete old;
            return true;
        }
        return erase(key < current->key ? current->left : current->right, key);
    }

    Node* find(Node* current, const string& key) const {
        while (current) {
            if (current->key == key) return current;
            current = key < current->key ? current->left : current->right;
        }
        return nullptr;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

public:
    Treap() : root(nullptr) {}

    bool insert(const string& key, uint64_t value) {
        string lowercase_key = key;
        transform(lowercase_key.begin(), lowercase_key.end(), lowercase_key.begin(), ::tolower);
        return insert(root, lowercase_key, value);
    }

    bool erase(const string& key) {
        string lowercase_key = key;
        transform(lowercase_key.begin(), lowercase_key.end(), lowercase_key.begin(), ::tolower);
        return erase(root, lowercase_key);
    }

    Node* find(const string& key) const {
        string lowercase_key = key;
        transform(lowercase_key.begin(), lowercase_key.end(), lowercase_key.begin(), ::tolower);
        return find(root, lowercase_key);
    }
};

int main() {
    Treap dictionary;
    string command, word;
    uint64_t number;

    while (cin >> command) {
        if (command == "+") {
            cin >> word >> number;
            if (dictionary.insert(word, number)) {
                cout << "OK\n";
            }
            else {
                cout << "Exist\n";
            }
        }
        else if (command == "-") {
            cin >> word;
            if (dictionary.erase(word)) {
                cout << "OK\n";
            }
            else {
                cout << "NoSuchWord\n";
            }
        }
        else {
            word = command;
            Node* result = dictionary.find(word);
            if (result) {
                cout << "OK: " << result->value << "\n";
            }
            else {
                cout << "NoSuchWord\n";
            }
        }
    }
    return 0;
}