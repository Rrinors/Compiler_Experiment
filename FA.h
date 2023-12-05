#ifndef FA_H
#define FA_H

#include <vector>
#include <string>

class FA : std::vector<std::vector<std::pair<int, char>>> {
public:
    int new_node();
    void add_edge(int u, int v, char c = 0);
    void show();
};

FA RE_to_NFA(std::string s);

#endif