#include "FA.h"
#include <cassert>
#include <functional>
#include <iostream>

std::vector<char> CHAR_SET = {
    'a', 'b', 'c', 'd', '0', '1', '2', '3'
};

int FA::new_node() {
    FA::emplace_back();
    return int(FA::size()) - 1;
}

void FA::add_edge(int u, int v, char c) {
    (FA::begin() + u)->emplace_back(v, c);
}

void FA::show() {
    std::cout << "```mermaid" << std::endl << "graph LR" << std::endl;
    for (int u = 0; u < FA::size(); u++) {
        for (auto [v, c] : *(FA::begin() + u)) {
            std::cout << u << "--";
            if (c) { std::cout << c << "--"; }
            std::cout << ">" << v << std::endl;
        }
    }
    std::cout << "```" << std::endl;
}

FA RE_to_NFA(std::string re) {
    int n = re.length();

    std::vector<int> stk, match(n, -1);
    for (int i = 0; i < n; i++) {
        if (re[i] == '(') {
            stk.push_back(i);
        } else if (re[i] == ')') {
            assert(!stk.empty());
            match[stk.back()] = i;
            stk.pop_back();
        }
    }

    FA nfa;
    std::function<int(int, int, int)> work = [&](int l, int r, int lead) {
        int lst_in = lead, lst_out = lead, sink = nfa.new_node();
        for (int i = l; i <= r; i++) {
            if (re[i] == '*') {
                if (lst_in == lst_out) { continue; }
                nfa.add_edge(lst_out, lst_in);
                lst_out = lst_in;
            } else if (re[i] == '|') {
                nfa.add_edge(lst_out, sink);
                lst_in = lst_out = lead;
            } else if (re[i] == '(') {
                lst_in = nfa.new_node();
                nfa.add_edge(lst_out, lst_in);
                lst_out = work(i + 1, match[i] - 1, lst_in);
                i = match[i];
            } else {
                assert(std::find(CHAR_SET.begin(), CHAR_SET.end(), re[i]) != CHAR_SET.end());
                lst_in = nfa.new_node();
                nfa.add_edge(lst_out, lst_in);
                lst_out = nfa.new_node();
                nfa.add_edge(lst_in, lst_out, re[i]);
            }
        }
        nfa.add_edge(lst_out, sink);
        return lst_out = sink;
    };

    work(0, n - 1, nfa.new_node());

    return nfa;
}