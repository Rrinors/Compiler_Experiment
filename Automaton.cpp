#include "Automaton.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <set>
#include <queue>
#include <map>

std::vector<char> CHAR_SET = {
    'a', 'b', 'c', 'd', '0', '1', '2', '3'
};

int Finite_Automaton::new_node() {
    Finite_Automaton::emplace_back();
    accept.emplace_back();
    return int(Finite_Automaton::size()) - 1;
}

void Finite_Automaton::add_edge(int u, int v, char c) {
    (Finite_Automaton::begin() + u)->emplace_back(v, c);
}

void Finite_Automaton::show() {
    if (Finite_Automaton::empty()) {
        return;
    }
    std::cout << "```mermaid" << std::endl << "graph LR" << std::endl;
    for (int u = 0; u < Finite_Automaton::size(); u++) {
        if (accept[u]) {
            std::cout << u << "(((" << u << ")))" << std::endl;
        } else {
            std::cout << u << "((" << u << "))" << std::endl;
        }
    }
    for (int u = 0; u < Finite_Automaton::size(); u++) {
        for (auto [v, c] : *(Finite_Automaton::begin() + u)) {
            std::cout << u << "--";
            if (c) { std::cout << c; }
            else { std::cout << "ε"; }
            std::cout << "-->" << v << std::endl;
        }
    }
    std::cout << "style " << start << " fill:#7FFF00" << std::endl;
    std::cout << "```" << std::endl;
}

Finite_Automaton RE_to_NFA(Regular_Expression re) {
    if (re.empty()) {
        return {};
    }

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

    Finite_Automaton nfa;
    auto work = [&](auto self, int l, int r, int lead) -> int {
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
                lst_out = self(self, i + 1, match[i] - 1, lst_in);
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

    work(work, 0, n - 1, nfa.new_node());
    nfa.start = 0;
    nfa.accept[1] = true;

    return nfa;
}

Finite_Automaton NFA_to_DFA(Finite_Automaton nfa) {
    if (nfa.empty()) {
        return {};
    }

    auto extend = [&](std::set<int> s) {
        std::set<int> res;
        std::queue<int> q;
        for (auto u : s) {
            res.insert(u);
            q.push(u);
        }
        while (!q.empty()) {
            auto u = q.front();
            q.pop();
            for (auto [v, w] : nfa[u]) {
                if (w == 0 && !res.contains(v)) {
                    res.insert(v);
                    q.push(v);
                }
            }
        }
        return res;
    };

    auto trans = [&](std::set<int> s, char c) {
        std::set<int> res;
        for (auto u : s) {
            for (auto [v, w] : nfa[u]) {
                if (w == c) { res.insert(v); }
            }
        }
        return extend(res);
    };

    auto check_accept = [&](std::set<int> s) {
        for (auto x : s) {
            if (nfa.accept[x]) { return true; }
        }
        return false;
    };

    Finite_Automaton dfa;
    std::map<std::set<int>, int> id;
    std::queue<std::set<int>> q;

    auto start = extend(std::set{nfa.start});
    dfa.accept[dfa.start = id[start] = dfa.new_node()] = check_accept(start);
    q.push(start);

    while (!q.empty()) {
        auto s = q.front();
        q.pop();
        for (auto c : CHAR_SET) {
            auto ns = trans(s, c);
            if (!ns.empty()) {
                if (!id.contains(ns)) {
                    dfa.accept[id[ns] = dfa.new_node()] = check_accept(ns);
                    q.push(ns);
                }
                dfa.add_edge(id[s], id[ns], c);
            }
        }
    }

    return minimize_DFA(dfa); 
}

Finite_Automaton minimize_DFA(Finite_Automaton dfa) {
    if (dfa.empty()) {
        return {};
    }
    
    int n = dfa.size();

    std::vector<int> bel(n);
    std::queue<std::set<int>> q;

    std::set<int> s1, s2;
    for (int i = 0; i < n; i++) {
        if (dfa.accept[i]) {
            s1.insert(i);
            bel[i] = 0;
        } else {
            s2.insert(i);
            bel[i] = 1;
        }
    }

    q.push(s1);
    q.push(s2);

    std::vector<std::set<int>> sets;
    int tot = 2;

    auto split = [&](std::set<int> s) {
        if (s.empty()) {
            return true;
        }
        for (auto c : CHAR_SET) {
            std::map<int, std::set<int>> mp;
            for (auto x : s) {
                bool splitted = false;
                for (auto [y, w] : dfa[x]) {
                    if (w == c) {
                        mp[bel[y]].insert(x);
                        splitted = true;
                        break;
                    }
                }
                if (!splitted) {
                    mp[-1].insert(x);
                }
            }
            if (mp.size() > 1) {
                for (auto [_, u] : mp) {
                    for (auto x : u) {
                        bel[x] = tot;
                    }
                    q.push(u);
                    tot++;
                }
                return true;
            }
        }
        return false;
    };

    while (!q.empty()) {
        auto s = q.front();
        q.pop();
        if (!split(s)) {
            sets.push_back(s);
        }
    }

    for (int i = 0; auto s : sets) {
        for (auto x : s) {
            bel[x] = i;
        }
        i++;
    }

    Finite_Automaton res;
    for (auto s : sets) {
        int id = res.new_node();
        res.accept[id] = dfa.accept[*s.begin()];
        for (auto x : s) {
            for (auto [y, w] : dfa[x]) {
                if (std::find(res[id].begin(), res[id].end(), std::pair(bel[y], w)) == res[id].end()) {
                    res[id].emplace_back(bel[y], w);
                }
            }
        }
    }

    res.start = bel[dfa.start];

    return res;
}