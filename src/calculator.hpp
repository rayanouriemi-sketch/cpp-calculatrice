#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

struct Token {
    enum class Type { Number, Variable, Function, Operator, LeftParen, RightParen };
    Type type;
    std::string text;  // nom de variable / fonction / opérateur
    double value = 0;  // pour les nombres
};

class CalcError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

std::vector<Token> tokenize(const std::string& expr);
// Algorithme Shunting-yard de Dijkstra : notation infixe -> notation polonaise inverse.
std::vector<Token> toRpn(const std::vector<Token>& tokens);

class Calculator {
public:
    Calculator();
    // Évalue une expression, ou une affectation de la forme "nom = expression".
    double evaluate(const std::string& line);
    const std::map<std::string, double>& variables() const { return vars_; }

private:
    double evalRpn(const std::vector<Token>& rpn) const;
    std::map<std::string, double> vars_;
};
