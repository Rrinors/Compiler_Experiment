#ifndef FA_H
#define FA_H

#include <vector>
#include <string>

struct FA : std::vector<std::vector<std::pair<int, char>>> {
    int start;
    std::vector<bool> accept;
    int new_node();
    void add_edge(int u, int v, char c = 0);
    void show();
};

FA RE_to_NFA(std::string s);

FA NFA_to_DFA(FA nfa);

#endif