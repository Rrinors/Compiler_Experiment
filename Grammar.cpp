#include "Grammar.h"
#include "Show.h"
#include <cassert>
#include <queue>
#include <algorithm>

Grammar rm_left_recursion(Grammar g) {
    std::vector<std::pair<std::string, std::set<std::string>>> r;
    auto rm_self_recursion = [&](std::set<std::string> p, std::string c) {
        auto q = c + char(39);
        std::set<std::string> a, b;
        for (auto s : p) {
            if (get_head(s) != c) {
                a.insert(s + q);
            } else {
                b.insert(rm_head(s) + q);
            }
        }
        if (b.empty()) { return p; }
        b.emplace();
        r.emplace_back(q, b);
        return a;
    };

    std::set<std::string> used;
    for (auto &[c, v] : g) {
        std::set<std::string> res;
        for (auto s : v) {
            assert(!s.empty());
            std::queue<std::string> q;
            q.push(s);
            std::set<std::string> add;
            while (!q.empty()) {
                auto u = q.front();
                q.pop();
                if (!used.contains(get_head(u))) {
                    add.insert(u);
                } else {
                    auto tmp = add_each(g[get_head(u)], rm_head(u));
                    for (auto x : tmp) {
                        q.push(x);
                    }
                }
            }
            res = res + add;
        }
        res = rm_self_recursion(res, c);
        std::swap(v, res);
        used.insert(c);
    }

    for (auto p : r) {
        g.insert(p);
    }
    
    return g;
}

template<typename T>
std::set<T> operator+(std::set<T> a, std::set<T> b) {
    for (auto x : b) {
        a.insert(x);
    }
    return a;
}

template<typename T>
std::set<T> add_each(std::set<T> a, T b) {
    std::set<T> res;
    for (auto x : a) {
        res.insert(x + b);
    }
    return res;
}

std::string get_head(std::string s) {
    if (s.size() > 1 && s[1] == 39) {
        return s.substr(0, 2);
    } else {
        return s.substr(0, 1);
    }
};

std::string rm_head(std::string s) {
    if (s.size() > 1 && s[1] == 39) {
        s.erase(0, 2);
    } else {
        s.erase(0, 1);
    }
    return s;
}

Grammar extract_LCF(Grammar g) {
    Grammar res;
    res.start = g.start;
    for (auto [c, v] : g) {
        Trie t;
        int cnt = 0;
        for (auto s : v) {
            t.add(s);
        }
        auto dfs = [&](auto self, int x, std::string cur, std::string parent) -> void {
            if (t[x].empty()) {
                res[parent].insert(cur);
                return;
            }
            if (t[x].size() > 1) {
                auto now = c;
                if (!cur.empty()) {
                    now = c + std::to_string(cnt++);
                    res[parent].insert(cur + now);
                }
                for (auto [w, y] : t[x]) {
                    self(self, y, w, now);
                }
            } else {
                auto [w, y] = *t[x].begin();
                self(self, y, cur + w, parent);
            }
        };
        dfs(dfs, 0, {}, c);
    }
    return res;
}

int Trie::new_node() {
    Trie::emplace_back();
    return Trie::size() - 1;
}

Trie::Trie() {
    Trie::new_node();
}

void Trie::add(std::string s) {
    int n = s.length(), p = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            continue;
        }
        int len = 1;
        while (i + len < n && s[i + len] >= '0' && s[i + len] <= '9') {
            len++;
        }
        auto cur = s.substr(i, len);
        if (!(*this)[p].contains(cur)) {
            (*this)[p][cur] = new_node();
        }
        p = (*this)[p][cur];
    }
}

std::map<std::string, std::set<std::string>> get_first_set(Grammar g) {
    std::map<std::string, std::set<std::string>> res;
    std::set<std::string> tmp;

    auto dfs = [&](auto self, std::string cur) -> bool {
        bool flag = true;
        for (auto s : g[cur]) {
            if (s.empty()) {
                res[cur].emplace();
                flag = false;
            }
            for (int i = 0; i < int(s.length()); i++) {
                if (s[i] == 39) {
                    continue;
                }
                std::string t = s.substr(i, 1);
                if (i + 1 < int(s.length()) && s[i + 1] == 39) {
                    t.push_back(s[i + 1]);
                }
                if (g.contains(t)) {
                    if (self(self, t)) {
                        break;
                    }
                } else {
                    tmp.insert(t);
                    break;
                }
            }
        }
        return flag;
    };

    for (auto [c, _] : g) {
        dfs(dfs, c);
        res[c] = res[c] + tmp;
        tmp.clear();
    }

    return res;
}

std::map<std::string, std::set<std::string>> get_follow_set(Grammar g) {
    auto first = get_first_set(g);

    std::map<std::string, std::set<std::string>> res;
    std::queue<std::string> q;
    for (auto [c, _] : g) {
        q.push(c);
        res[c] = {};
    }
    res[g.start].insert("$");

    while (!q.empty()) {
        auto c = q.front();
        q.pop();
        for (auto s : g[c]) {
            for (int i = 0; i < int(s.length()); i++) {
                if (s[i] == 39) {
                    continue;
                }
                std::string cur{s[i]};
                if (i + 1 < int(s.length()) && s[i + 1] == 39) {
                    cur.push_back(s[i + 1]);
                }
                if (g.contains(cur)) {
                    int j = i + cur.size();
                    if (j == int(s.length())) {
                        bool add = false;
                        for (auto x : res[c]) { 
                            if (!res[cur].contains(x)) {
                                add = true;
                                res[cur].insert(x);
                            }
                        }
                        if (add) { q.push(cur); }
                    } else {
                        std::string y{s[j]};
                        if (j + 1 < int(s.length()) && s[j + 1] == 39) {
                            y.push_back(s[j + 1]);
                        }
                        if (g.contains(y)) {
                            if (j + int(y.length()) == int(s.length())) {
                                bool y_to_empty = false, add = false;
                                for (auto x : first[y]) {
                                    if (x.empty()) {
                                        y_to_empty = true;
                                    } else if (!res[cur].contains(x)) {
                                        add = true;
                                        res[cur].insert(x);
                                    }
                                }
                                if (y_to_empty) {
                                    for (auto x : res[c]) {
                                        if (!res[cur].contains(x)) {
                                            add = true;
                                            res[cur].insert(x);
                                        }
                                    }
                                }
                                if (add) { q.push(cur); }
                                add = false;
                                for (auto x : res[c]) {
                                    if (!res[y].contains(x)) {
                                        add = true;
                                        res[y].insert(x);
                                    }
                                }
                                if (add) { q.push(y); }
                            } else {
                                bool add = false;
                                for (auto x : first[y]) {
                                    if (!x.empty() && !res[cur].contains(x)) {
                                        add = true;
                                        res[cur].insert(x);
                                    }
                                }
                                if (add) { q.push(cur); }
                            }
                        } else if (!res[cur].contains(y)) {
                            res[cur].insert(y);
                            q.push(cur);
                        }
                    }
                }
            }
        }
    }

    return res;
}

std::map<std::pair<std::string, std::string>, std::set<std::string>> get_select_set(Grammar g) {
    auto first = get_first_set(g);
    auto follow = get_follow_set(g);

    std::map<std::pair<std::string, std::string>, std::set<std::string>> res;
    
    for (auto [c, v] : g) {
        for (auto s : v) {
            std::set<std::string> set;
            for (int i = 0; i < int(s.length()); i++) {
                if (s[i] == 39) { continue; }
                std::string cur{s[i]};
                if (i + 1 < int(s.length()) && s[i + 1] == 39) {
                    cur.push_back(s[i + 1]);
                }
                if (g.contains(cur)) {
                    set = set + first[cur];
                } else {
                    set.insert(cur);
                }
                if (i + int(cur.length()) < int(s.length()) && set.contains("")) {
                    set.erase("");
                } else {
                    break;
                }
            }
            if (set.empty()) {
                set = set + follow[c];
            } else if (set.contains("")) {
                set.erase("");
                set = set + follow[c];
            }
            res[{c, s}] = set;
        }
    }

    return res;
}

bool check_LL1(Grammar g) {
    auto select = get_select_set(g);

    std::map<std::string, std::set<std::string>> f;
    
    for (auto [p, s] : select) {
        if (int(g[p.first].size()) == 1) { continue; }
        if (!f.contains(p.first)) {
            f[p.first] = s;
        } else {
            std::set<std::string> tmp;
            for (auto x : f[p.first]) {
                if (s.contains(x)) {
                    tmp.insert(x);
                }
            }
            std::swap(f[p.first], tmp);
        }
    }

    for (auto [_, g] : f) {
        if (!g.empty()) {
            return false;
        }
    }
    
    return true;
}

std::map<std::string, std::map<std::string, std::string>> get_LL1_PAT(Grammar g) {
    auto select = get_select_set(g);
    
    if (!check_LL1(g)) {
        return {};
    }

    show("select");
    show(select);
    show("");

    std::map<std::string, std::map<std::string, std::string>> res;
    std::set<std::string> all;
    for (auto [p, s] : select) {
        for (auto c : s) {
            res[p.first][c] = p.second;
            all.insert(c);
        }
    }

    for (auto c : all) {
        for (auto &[_, mp] : res) {
            if (!mp.contains(c)) {
                mp[c] = "EMPTY";
            }
        }
    }

    return res;
}