#include "Automaton.h"
#include "Grammar.h"

int main() {
    // Regular_Expression re = "a*b*(|c|d*)|((01)*23)*";
    // Regular_Expression re = "a(bc|dc)*";
    // auto nfa = RE_to_NFA(re);
    // nfa.show();
    // auto dfa = NFA_to_DFA(nfa);
    // dfa.show();

    Grammar g;
    g["S"].insert("Qc");
    g["S"].insert("c");
    g["Q"].insert("Rb");
    g["Q"].insert("b");
    g["R"].insert("Sa");
    g["R"].insert("a");
    show(g);
    std::cout << "====\n";
    g = rm_left_recursion(g);
    show(g);

    return 0;
}