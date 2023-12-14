#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <set>
#include <map>
#include <vector>

using Grammar = std::map<std::string, std::set<std::string>>;

Grammar rm_left_recursion(Grammar g);

template<typename T>
std::set<T> operator+(std::set<T> a, std::set<T> b);

template<typename T>
std::set<T> add_each(std::set<T> a, T b);

std::string get_head(std::string s);

std::string rm_head(std::string s);

void show(Grammar g);

Grammar extract_LCF(Grammar g);

struct Trie : std::vector<std::map<std::string, int>> {
    int new_node();
    void add(std::string s);
    Trie();
};

std::map<std::string, std::set<std::string>> get_first_set(Grammar g);

#endif