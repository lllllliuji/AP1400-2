#ifndef BST_H
#define BST_H
#include <functional>
#include <initializer_list>
#include <iostream>
#include <ostream>

class BST {
   public:
    class Node {
       public:
        Node(int value, Node *left, Node *right) : value(value), left(left), right(right) {}
        Node() : value(0), left(nullptr), right(nullptr){};
        Node(const Node &node) : value(node.value), left(node.left), right(node.right) {}

        friend std::ostream &operator<<(std::ostream &out, const Node &node) {
            out << &node << "\t"
                << "=> value: " << node.value << "\t"
                << "left: " << node.left << "\t"
                << "right: " << node.right;
            return out;
        }
        friend bool operator<(const Node &node, int value) { return node.value < value; }
        friend bool operator>(const Node &node, int value) { return node.value > value; }
        friend bool operator==(const Node &node, int value) { return node.value == value; }
        friend bool operator<=(const Node &node, int value) { return node < value || node == value; }
        friend bool operator>=(const Node &node, int value) { return node > value || node == value; }
        friend bool operator<(int value, const Node &node) { return node > value; }
        friend bool operator>(int value, const Node &node) { return node < value; };
        friend bool operator==(int value, const Node &node) { return node == value; }
        friend bool operator<=(int value, const Node &node) { return node >= value; }
        friend bool operator>=(int value, const Node &node) { return node <= value; }
        int value;
        Node *left;
        Node *right;
    };

    BST() : count(0), root(nullptr) {}
    BST(const BST &);
    BST(BST &&);
    BST(std::initializer_list<int>);

    BST &operator=(const BST &);
    BST &operator=(BST &&);
    BST operator++();
    BST operator++(int);

    Node *&get_root();
    void bfs(std::function<void(Node *&node)> func);
    size_t length();
    bool add_node(int value);
    Node **find_node(int value);
    Node **find_parrent(int value);
    Node **find_successor(int value);
    bool delete_node(int value);

    friend std::ostream &operator<<(std::ostream &out, const BST &bst) {
        std::function<void(Node *)> dfs = [&](Node *root) {
            if (!root) {
                return;
            }
            out << *root << std::endl;
            dfs(root->left);
            dfs(root->right);
        };
        dfs(bst.root);
        return out;
    }

   private:
    size_t count;
    Node *root;
};

#endif  // BST_H