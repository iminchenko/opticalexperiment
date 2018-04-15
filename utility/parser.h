# pragma once

#include <string>
#include <list>
#include <utility>

void insertMathConsts(std::string &expr);

double evaluateExprassion(std::string expr,
                          const std::list<std::pair<std::string, double>> &variables);