#include "Automaton.h"
#include "Grammar.h"
#include "Show.h"

// 正则表达式转NFA、NFA转DFA、DFA最小化
void E1() {
    Regular_Expression re = "a(bc|dc)*";
    auto nfa = RE_to_NFA(re);
    nfa.show();
    auto dfa = NFA_to_DFA(nfa);
    dfa.show();
}

// 消除左递归
void E2() {
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
}

// 提取左公因子
void E3() {
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
}

// FIRST集
void E4() {
    Grammar g;
    g.start = "A";
    // g["A"].insert("Ba");
    // g["A"].insert("b");
    // g["B"].insert("Ac");
    g["A"].insert("");
    // g = rm_left_recursion(g);
    show(g);
    std::cout << "====\n";
    auto ans = get_first_set(g);
    show(ans);
}

// FOLLOW集
void E5() {
    Grammar g;
    g.start = "A";
    g["A"].insert("aBA");
    g["A"].insert("d");
    g["B"].insert("Ca");
    g["C"].insert("d");
    g = rm_left_recursion(g);
    show(g);
    std::cout << "====\n";
    auto ans = get_follow_set(g);
    show(ans);
}

// SELECT集
void E6() {
    Grammar g;
    g.start = "E";
    g["E"].insert("TA");
    g["A"].insert("+TA");
    g["A"].insert("");
    g["T"].insert("FB");
    g["B"].insert("*FB");
    g["B"].insert("");
    g["F"].insert("i");
    g["F"].insert("(E)");
    // g = rm_left_recursion(g);
    show(g);
    std::cout << "====\n";
    auto ans = get_select_set(g);
    show(ans);
}

// 判断LL1文法及预测表
void E7() {
    Grammar g;
    g.start = "S";
    g["S"].insert("AaS");
    g["S"].insert("BbS");
    g["S"].insert("d");
    g["A"].insert("a");
    g["B"].insert("");
    g["B"].insert("c");
    show(g);
    std::cout << "====\n";
    std::cout << "LL1: " << check_LL1(g) << std::endl;
    auto ans = get_LL1_PAT(g);
    show(ans);
}

int main() {
    // E1();
    // E2();
    // E3();
    // E4();
    // E5();
    // E6();
    E7();

    return 0;
}