#include "RE_to_NFA.h"

int main() {
    std::string s = "ab*(a*|(ab)*)*";
    auto nfa = RE_to_NFA(s);
    nfa.print();
    return 0;
}