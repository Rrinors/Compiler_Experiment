#include "Grammar.h"
#include <cassert>
#include <queue>
// #include "Debug.h"

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

void show(Grammar g) {
    for (auto [c, v] : g) {
        std::cout << c << "->";
        for (auto s : v) {
            if (s.empty()) std::cout << "ε";
            else std::cout << s;
            if (s != *v.rbegin()) {
                std::cout << '|';
            }
        }
        std::cout << std::endl;
    }
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

// std::map<char, std::set<char>> get_first_set(Grammar g) {
//     std::map<char, std::set<char>> res;
//     auto dfs = [&](auto self, char cur) -> void {
//     };
//     for (auto [c, v] : g) {
//         for (auto s : v) {   
//         }
//     }
//     return res;
// }