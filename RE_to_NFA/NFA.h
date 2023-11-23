#ifndef NFA_H
#define NFA_H

#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>

struct Node {
    int id;
    bool vis;
    std::vector<std::pair<Node *, int>> next;

    Node(int id) : id(id), vis{false}, next{} {}
    void add_edge(Node *t, int c = -1) {
        next.emplace_back(t, c);
    }
};

class NFA {
private:
    int cnt_nodes = 0;
    Node *S;
public:
    NFA() {
        S = new_node();
    }
    Node *new_node() {
        Node *t = new Node(cnt_nodes++);
        return t;
    }
    Node *start() {
        return S;
    }
    void print() {
        auto dfs = [&](auto self, Node *u, bool p) -> void {
            u->vis = p;
            for (auto [v, c] : u->next) {
                if (v->vis != p) {
                    self(self, v, p);
                }
                if (p) {
                    std::cout << u->id << "--";
                    if (c != -1) {
                        std::cout << char(c + 'a') << "--";
                    }
                    std::cout << '>' << v->id << std::endl;
                }
            }
        };
        dfs(dfs, start(), true);
        dfs(dfs, start(), false);
    }
};

#endif