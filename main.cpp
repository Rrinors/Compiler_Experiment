#include "FA.h"

int main() {
    // std::string re = "a*b*(|c|d*)|((01)*23)*";
    std::string re = "(a|b)*c";
    auto nfa = RE_to_NFA(re);
    nfa.show();
    auto dfa = NFA_to_DFA(nfa);
    dfa.show();
    return 0;
}