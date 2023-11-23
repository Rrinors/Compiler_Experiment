#ifndef RE_TO_NFA_H
#define RE_TO_NFA_H

#include "NFA.h"
#include <string>

NFA RE_to_NFA(const std::string &re) {
    int n = re.length();
    
    std::vector<int> stk, match(n, -1);
    for (int i = 0; i < n; i++) {
        if (re[i] == '(') {
            stk.push_back(i);
        } else if (re[i] == ')') {
            if (stk.empty()) {
                std::cerr << "Bracket mismatched" << std::endl;
                std::exit(-1);
            }
            match[stk.back()] = i;
            stk.pop_back();
        }
    }

    NFA nfa;
    Node *lst_out = nfa.start();
    auto work = [&](auto self, int l, int r, Node *lead, Node *lst_in) -> void {
        auto sink = nfa.new_node();
        for (int i = l; i <= r; i++) {
            if (re[i] == '*') {
                lst_in->add_edge(lst_out);
                lst_out->add_edge(lst_in);
            } else if (re[i] == '|') {
                lst_out->add_edge(sink);
                lst_in = lst_out = lead;
            } else if (re[i] == '(') {
                lst_in = lst_out;
                self(self, i + 1, match[i] - 1, lst_out, lst_out);
                i = match[i];
            } else {
                if (re[i] < 'a' || re[i] > 'z') {
                    std::cerr << "Out of character set" << std::endl;
                    std::exit(-1);
                }
                auto t = nfa.new_node();
                lst_out->add_edge(t, re[i] - 'a');
                lst_in = lst_out;
                lst_out = t;
            }
        }
        lst_out->add_edge(sink);
        lst_out = sink;
    };

    work(work, 0, n - 1, nfa.start(), nfa.start());

    return nfa;
}

#endif