#include "bst.h"
#include <cassert>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <queue>
#include <stack>

BST::Node *&BST::get_root() { return root; }

void BST::bfs(std::function<void(Node *&)> func) {
    std::queue<Node *> q;
    q.push(root);
    while (!q.empty()) {
        auto t = q.front();
        q.pop();
        func(t);
        if (t->left) q.push(t->left);
        if (t->right) q.push(t->right);
    }
}

size_t BST::length() { return count; }

bool BST::add_node(int value) {
    std::function<bool(Node *&)> insert_helper = [&](Node *&root) -> bool {
        if (root == nullptr) {
            root = new Node(value, nullptr, nullptr);
            return true;
        }
        if (value == root->value) return false;
        if (value < root->value) {
            return insert_helper(root->left);
        }
        return insert_helper(root->right);
    };
    if (!insert_helper(root)) {
        return false;
    }
    count++;
    return true;
}

BST::Node **BST::find_node(int value) {
    if (!root) {
        return nullptr;
    }
    if (root->value == value) {
        return &root;
    }
    Node **node_ptr_ptr = nullptr;
    bool flag = false;
    std::function<void(Node *&, Node **)> find_node_helper = [&](Node *&cur, Node **cur_ptr) {
        if (cur == nullptr) {
            return;
        }
        if (flag) {
            return;
        }
        if (value == cur->value) {
            node_ptr_ptr = cur_ptr;
            flag = true;
            return;
        }
        if (value < cur->value) {
            find_node_helper(cur->left, &cur->left);
            return;
        }
        if (value > cur->value) {
            find_node_helper(cur->right, &cur->right);
            return;
        }
    };
    find_node_helper(root, &root);
    return node_ptr_ptr;
}

BST::Node **BST::find_parrent(int value) {
    // root has no parent, local variable
    if (!root) {
        return nullptr;
    }
    Node **ans = nullptr;
    bool flag = false;
    std::function<void(Node *&, Node **)> find_parrent_helper = [&](Node *&cur, Node **parent_ptr) {
        if (root == nullptr) {
            return;
        }
        if (flag) {
            return;
        }
        if (value == cur->value) {
            ans = parent_ptr;
            flag = true;
            return;
        }
        if (value < cur->value) {
            find_parrent_helper(cur->left, &cur);
        } else {
            find_parrent_helper(cur->right, &cur);
        }
    };
    find_parrent_helper(root, nullptr);
    return ans;
}

BST::Node **BST::find_successor(int value) {
    if (!root) return nullptr;
    bool flag = false;
    Node *last_visited = nullptr;
    Node *last_visited_parent = nullptr;
    int is_left = 0;
    std::function<void(Node *&, Node * parent, int)> find_successor_helper = [&](Node *&cur, Node *parent,
                                                                                 int from_left) {
        if (!cur) {
            return;
        }
        if (flag) {
            return;
        }
        find_successor_helper(cur->left, cur, 1);
        if (last_visited != nullptr && cur->value >= value) {
            flag = true;
            return;
        }
        last_visited = cur;
        last_visited_parent = parent;
        is_left = from_left;
        find_successor_helper(cur->right, cur, 0);
    };
    find_successor_helper(root, nullptr, 0);
    if (!flag) {
        return nullptr;
    }
    if (last_visited == root) {
        return &root;
    }
    return is_left == 0 ? &last_visited_parent->right : &last_visited_parent->left;
}

bool BST::delete_node(int value) {
    auto node_ptr_ptr = find_node(value);
    if (node_ptr_ptr == nullptr) {
        return false;
    }
    Node *node_to_delete = nullptr;
    std::function<Node *(Node *, int)> delete_helper = [&](Node *cur, int val) -> Node * {
        if (!cur) {
            return nullptr;
        }
        if (val < cur->value) {
            cur->left = delete_helper(cur->left, val);
            return cur;
        }
        if (val > cur->value) {
            cur->right = delete_helper(cur->right, val);
            return cur;
        }
        assert(cur->value == val);
        node_to_delete = cur;
        if (!cur->left && !cur->right) {
            return nullptr;
        }
        if (cur->left && cur->right) {
            auto successor_ptr_ptr = find_successor(val);
            cur->value = (*successor_ptr_ptr)->value;
            cur->left = delete_helper(cur->left, cur->value);
            return cur;
        }
        return cur->left ? cur->left : cur->right;
    };
    root = delete_helper(root, value);
    count--;
    delete node_to_delete;
    return true;
}

BST::BST(const BST &bst) {
    std::function<void(Node *&, Node *)> copy_helper = [&](Node *&a, Node *b) {
        if (b == nullptr) {
            return;
        }
        a = new Node(b->value, nullptr, nullptr);
        copy_helper(a->left, b->left);
        copy_helper(a->right, b->right);
    };
    copy_helper(root, bst.root);
    count = bst.count;
}

BST::BST(BST &&rhs) {
    if (this == &rhs) {
        return;
    }
    root = rhs.root;
    count = rhs.count;
    // bfs([](Node *&node) { printf("%d\n", node->value); });
    rhs.root = nullptr;
}

BST &BST::operator=(const BST &bst) {
    if (this == &bst) {
        return *this;
    }
    std::function<void(Node *&, Node *)> copy_helper = [&](Node *&a, Node *b) {
        if (b == nullptr) {
            return;
        }
        a = new Node(b->value, nullptr, nullptr);
        copy_helper(a->left, b->left);
        copy_helper(a->right, b->right);
    };
    copy_helper(root, bst.root);
    count = bst.count;
    return *this;
}

BST &BST::operator=(BST &&rhs) {
    if (this == &rhs) {
        return *this;
    }
    root = rhs.root;
    count = rhs.count;
    rhs.root = nullptr;
    return *this;
}

BST::BST(std::initializer_list<int> li) : count(0), root(nullptr) {
    for (int x : li) {
        add_node(x);
    }
}

BST BST::operator++() {
    bfs([](Node *&node) { node->value++; });
    return *this;
}

BST BST::operator++(int) {
    auto tmp = *this;
    // bfs([](Node *&node) { printf("%d\n", node->value); });
    bfs([](Node *&node) { node->value++; });
    return tmp;
}