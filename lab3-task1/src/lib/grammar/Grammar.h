#ifndef LAB3_TASK1_GRAMMAR_H
#define LAB3_TASK1_GRAMMAR_H

#include "../graph/CGraph.h"
#include <map>
#include <set>
#include <string>

std::map<std::string, std::set<std::string>> ParseRules(std::istream& in);

CGraph<std::string, std::string> BuildLeftGrammarGraph(const std::map<std::string, std::set<std::string>>& rules);

#endif // LAB3_TASK1_GRAMMAR_H
