#include "FA.h"

int main() {
    std::string re = "a*b*(|c|d*)|((01)*23)*";
    auto nfa = RE_to_NFA(re);
    nfa.show();
    return 0;
}