#include "Automaton.h"

int main() {
    // Regular_Expression re = "a*b*(|c|d*)|((01)*23)*";
    Regular_Expression re = "a(bc|dc)*";
    auto nfa = RE_to_NFA(re);
    nfa.show();
    auto dfa = NFA_to_DFA(nfa);
    dfa.show();
    return 0;
}