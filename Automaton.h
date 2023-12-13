#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <vector>
#include <string>

using Regular_Expression = std::string;

struct Finite_Automaton : std::vector<std::vector<std::pair<int, char>>> {
    int start;
    std::vector<bool> accept;
    int new_node();
    void add_edge(int u, int v, char c = 0);
    void show();
};

Finite_Automaton RE_to_NFA(Regular_Expression s);

Finite_Automaton NFA_to_DFA(Finite_Automaton nfa);

Finite_Automaton minimize_DFA(Finite_Automaton dfa);

#endif