#include "Show.h"
#include <iostream>

void show(Grammar g) {
    std::cout << "start: " << g.start << std::endl;
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

void show(std::map<std::string, std::set<std::string>> mp) {
    for (auto [c, s] : mp) {   
        std::cout << c << ": [";
        for (auto x : s) {
            if (x.empty()) std::cout << "ε";
            else std::cout << x;
            if (x != *s.rbegin()) {
                std::cout << ",";
            }
        }
        std::cout << "]\n";
    }
}

void show(std::map<std::pair<std::string, std::string>, std::set<std::string>> mp) {
    for (auto [c, s] : mp) {
        std::cout << c.first << "->" << (c.second.empty() ? "ε" : c.second) << ": [";
        for (auto x : s) {
            if (x.empty()) std::cout << "ε";
            else std::cout << x;
            if (x != *s.rbegin()) {
                std::cout << ",";
            }
        }
        std::cout << "]\n";
    }
}

void show(std::string s) {
    std::cout << s << "\n";
}

void show(std::set<std::string> s) {
    std::cout << "[";
    for (auto x : s) {
        std::cout << x;
        if (x != *s.rbegin()) { std::cout << ","; }
    }
    std::cout << "]\n";
}

void show(std::map<std::string, std::map<std::string, std::string>> mp) {
    if (mp.empty()) { return; }
    std::cout << "| |";
    for (auto [s, _] : mp.begin()->second) {
        std::cout << " " << s << " |";
    }
    std::cout << "\n";
    std::cout << "| -- |";
    for (auto [_, __] : mp.begin()->second) {
        std::cout << " -- |";
    }
    std::cout << "\n";
    for (auto [c, smp] : mp) {
        std::cout << "| " << c << " |";
        for (auto [_, s] : smp) {
            if (s == "EMPTY")
            std::cout << " ";
            if (s != "EMPTY") {
                std::cout << c << "->";
                if (!s.empty()) std::cout << s;
                else std::cout << "ε";
            }
            std::cout << " |";
        }
        std::cout << "\n";
    }
}