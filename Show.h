#include "Automaton.h"
#include "Grammar.h"
#include <iostream>

void show(Grammar g);

void show(std::map<std::string, std::set<std::string>> mp);

void show(std::map<std::pair<std::string, std::string>, std::set<std::string>> mp);

void show(std::map<std::string, std::map<std::string, std::string>> mp);

void show(std::string s);

void show(std::set<std::string> s);