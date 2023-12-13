#include "Automaton.h"
#include "Grammar.h"
#include <iostream>

int main() {
    // 正则表达式转 NFA、NFA 转 DFA 和 DFA 最小化测试代码
    // Regular_Expression re = "a*b*(|c|d*)|((01)*23)*";
    // Regular_Expression re = "a(bc|dc)*";
    // auto nfa = RE_to_NFA(re);
    // nfa.show();
    // auto dfa = NFA_to_DFA(nfa);
    // dfa.show();

    // 消除左递归测试代码
    // Grammar g;
    // g["S"].insert("Qc");
    // g["S"].insert("c");
    // g["Q"].insert("Rb");
    // g["Q"].insert("b");
    // g["R"].insert("Sa");
    // g["R"].insert("a");
    // show(g);
    // std::cout << "====\n";
    // g = rm_left_recursion(g);
    // show(g);

    // 提取左公因子测试代码
    Grammar g;
    g["A"].insert("Bcd");
    g["A"].insert("Bce");
    g["A"].insert("Be");
    g["B"].insert("acd");
    g["B"].insert("bcd");
    show(g);
    std::cout << "====\n";
    g = extract_LCF(g);
    show(g);

    return 0;
}