#include "Automaton.h"
#include "Grammar.h"
#include <iostream>

void show(Grammar g);

void show(std::map<std::string, std::set<std::string>> mp);

void show(std::map<std::pair<std::string, std::string>, std::set<std::string>> mp);