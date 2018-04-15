#include <iostream>
#include <string>
#include <array>
#include <cctype>
#include <utility>
#include <list>
#include <algorithm>
#include <sstream>
#include <experimental/array>
#include <experimental/string_view>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::array;
using std::list;
using std::pair;
using std::make_pair;
using std::experimental::make_array;
using std::experimental::string_view;

// символы, которые не могут быть посредине операторов
// явное указание длины строки, чтобы был `constexpr`
constexpr auto delimeters = make_array(',', '.', '(', ')',   // NOLINT
                                       '+', '-', '*', '/');

// операторы с приоритетом
enum opType {
    OP_PLUS = 0,
    OP_MINUS,
    OP_MULT,
    OP_DIV,
    OP_DEGREE,
    OP_SIN,
    OP_COS,
    OP_LOG,
    OP_EXP,
    OP_SQRT,
    OP_MINUS_UNARY,
    OP_UNKNOWN,
};

constexpr auto unaryOps = make_array(OP_MINUS_UNARY, OP_SIN, OP_COS, // NOLINT
                                     OP_LOG, OP_SQRT, OP_EXP);

constexpr auto mathConsts = make_array(make_pair("M_E", M_E),        // NOLINT
                                       make_pair("M_LOG2E", M_LOG2E),
                                       make_pair("M_LOG10E", M_LOG10E),
                                       make_pair("M_LN2", M_LN2),
                                       make_pair("M_LN10", M_LN10),
                                       make_pair("M_PI", M_PI),
                                       make_pair("M_PI_2", M_PI_2),
                                       make_pair("M_PI_4", M_PI_4),
                                       make_pair("M_1_PI", M_1_PI),
                                       make_pair("M_2_PI", M_2_PI),
                                       make_pair("M_2_SQRTPI", M_2_SQRTPI),
                                       make_pair("M_SQRT2", M_SQRT2),
                                       make_pair("M_SQRT1_2", M_SQRT1_2));

template <class T>
void insertVariables(string &exp, const T &vars) {
    for (const auto& iter : vars) {
        stringstream ss;
        ss << '(' << iter.second << ')';

        size_t found = exp.find(iter.first);
        while (found != string::npos) {
            exp.replace(found, string_view(iter.first).size(), ss.str());

            found = exp.find(iter.first, found + 1);
        }
    }
}

void insertMathConsts(string &exp) {
    insertVariables(exp, mathConsts);
}

void removeSpaces(string &str) {
    string noSpaces;

    noSpaces.reserve(str.size());

    for (const auto &c : str) {
        if (c != ' ')
            noSpaces.push_back(c);
    }

    noSpaces.reserve(noSpaces.size());

    str = std::move(noSpaces);
}

inline bool isDelimeter(char c) {
    return std::find(delimeters.cbegin(),
                     delimeters.cend(),
                     c) != delimeters.cend();
}

inline bool isUnary(opType op) {
    return unaryOps.cend()!= std::find(unaryOps.cbegin(),
                                       unaryOps.cend(),
                                       op);
}

inline bool isBinary(opType op) {
    return op == OP_MINUS || !isUnary(op);
}

// Определяет, что за оператор на позиции. Перемещает pos на место после
// операторa
opType operatorAt(string::const_iterator &pos,
                  const string::const_iterator &end) {
    if (pos == end) {
        return OP_UNKNOWN;
    }

    if (isDelimeter(*pos)) {
        char op = *pos;
        ++pos;
        switch (op) {
            case '+':
                return OP_PLUS;
            case '-':
                return OP_MINUS;
            case '/':
                return OP_DIV;
            case '*':
                return OP_MULT;
            case '^':
                return OP_DEGREE;
            default:
                // do nothing
                break;
        }
    }

    auto start = pos;

    while (++pos != end && !isDelimeter(*pos));

    string op(start, pos);

    // TODO: возможно подобное сравнение неоптимально для строк
    if (op == "sin") {
        return OP_SIN;
    } else if (op == "cos") {
        return OP_COS;
    } else if (op == "log") {
        return OP_LOG;
    } else if (op == "exp") {
        return OP_EXP;
    } else if (op == "sqrt") {
        return OP_SQRT;
    }

    return OP_UNKNOWN;
}

// ищет наименее приоритетный оператор, возвращает позицию
string::const_iterator lessPriorityOperator(const string::const_iterator &begin,
                                            const string::const_iterator &end) {
    auto pos = begin;

    int brCount = 0;

    opType opLesser = OP_UNKNOWN;
    auto iterLesser = begin;

    while (pos != end) {
        auto iterCur = pos;
        opType opCur = operatorAt(pos, end);

        // TODO: это костыль
        if (iterCur == begin && opCur == OP_MINUS)
            opCur = OP_MINUS_UNARY;

        if (opCur <= opLesser) {
            opLesser = opCur;
            iterLesser = iterCur;
        }

        if (pos != end && *pos == '(') {
            ++brCount;
            ++pos;
            while (brCount && pos != end) {
                if (*pos == '(') {
                    ++brCount;
                } else if (*pos == ')') {
                    --brCount;
                }
                ++pos;
            }
        }
    }

    return iterLesser;
};

// находится ли строка в скобках
bool inBrackets(const string::const_iterator &begin,
                const string::const_iterator &end) {
    int brCount = 0;

    for (auto iter = begin; iter != end; ++iter) {
        if (*iter == '(') {
            ++brCount;
            if (brCount == 0) {
                // значит, скобки уходили в минус
                throw "incorrect brackets";
            }
        } else if (*iter == ')') {
            --brCount;
            if (!brCount) {
                return *begin == '(' && ++iter == end;
            }
        }
    }

    if (brCount != 0)
        throw "incorrect brackets";

    return false;
}

// применение унарного оператора
double calculate(double rhs, opType op) {
    switch (op) {
        case OP_MINUS_UNARY:
            return -rhs;
        case OP_SQRT:
            return sqrt(rhs);
        case OP_LOG:
            return log(rhs);
        case OP_COS:
            return cos(rhs);
        case OP_SIN:
            return sin(rhs);
        case OP_EXP:
            return exp(rhs);
        default:
            throw "unary operator expected";
    }
}

// применение бинарного оператора
double calculate(double lhs, double rhs, opType op) {
    switch (op) {
        case OP_PLUS:
            return lhs + rhs;
        case OP_MINUS:
            return lhs - rhs;
        case OP_DIV:
            return lhs / rhs;
        case OP_MULT:
            return lhs * rhs;
        default:
            throw "binary operator expected";
    }
}

double calculate(string::const_iterator begin, string::const_iterator end) {
    if (begin >= end)
        return 0;

    if (inBrackets(begin, end)) {
        return calculate(++begin, --end);
    }

    auto pos = lessPriorityOperator(begin, end);

    // начало оператора
    auto start = pos;

    opType op = operatorAt(pos, end);

    // TODO: это костыль
    if (op == OP_MINUS && start==begin)
        op = OP_MINUS_UNARY;

    // если оператор не найден
    if (pos == end) {
        return std::stod(string(begin, end));
    }

    if (op == OP_UNKNOWN)
        throw "unknown operator";

    if (isUnary(op)) {
        // если что-то есть перед унарным оператором
        if (start != begin) {
            throw "incorrent using of unary operator";
        }

        return calculate(calculate(pos, end), op);
    } else if (isBinary(op)) {
        // если ничего нет перед бинарным оператором
        if (start == begin) {
            throw "incorrect using of binary operator";
        }

        return calculate(calculate(begin, start), calculate(pos, end), op);
    }
}

double evaluateExprassion(string expr,
                          const list<pair<string, double>> &variables) {
    removeSpaces(expr);
    insertVariables(expr, variables);
    return calculate(expr.begin(), expr.end());
}
