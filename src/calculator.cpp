#include "calculator.hpp"

#include <cmath>
#include <functional>

namespace {

using Type = Token::Type;

struct OpInfo {
    int precedence;
    bool rightAssoc;
};

// "neg" est le moins unaire. Sa priorité est inférieure à "^" : -2^2 = -4.
const std::map<std::string, OpInfo> kOperators = {
    {"+", {1, false}}, {"-", {1, false}}, {"*", {2, false}}, {"/", {2, false}},
    {"%", {2, false}}, {"neg", {3, true}}, {"^", {4, true}},
};

double checkedSqrt(double x) {
    if (x < 0) throw CalcError("racine carrée d'un nombre négatif");
    return std::sqrt(x);
}

double checkedLn(double x) {
    if (x <= 0) throw CalcError("logarithme d'un nombre négatif ou nul");
    return std::log(x);
}

const std::map<std::string, std::function<double(double)>> kFunctions = {
    {"sqrt", checkedSqrt},
    {"ln", checkedLn},
    {"abs", [](double x) { return std::fabs(x); }},
    {"sin", [](double x) { return std::sin(x); }},
    {"cos", [](double x) { return std::cos(x); }},
    {"tan", [](double x) { return std::tan(x); }},
    {"exp", [](double x) { return std::exp(x); }},
};

void popOperatorsFor(const OpInfo& cur, std::vector<Token>& stack, std::vector<Token>& output) {
    while (!stack.empty() && stack.back().type == Type::Operator) {
        const OpInfo& top = kOperators.at(stack.back().text);
        if (top.precedence < cur.precedence || (top.precedence == cur.precedence && cur.rightAssoc)) return;
        output.push_back(stack.back());
        stack.pop_back();
    }
}

}  // namespace

std::vector<Token> toRpn(const std::vector<Token>& tokens) {
    std::vector<Token> output, stack;
    for (const Token& t : tokens) {
        switch (t.type) {
            case Type::Number:
            case Type::Variable:
                output.push_back(t);
                break;
            case Type::Function:
            case Type::LeftParen:
                stack.push_back(t);
                break;
            case Type::Operator:
                if (t.text != "neg") popOperatorsFor(kOperators.at(t.text), stack, output);  // préfixe : ne dépile rien
                stack.push_back(t);
                break;
            case Type::RightParen:
                while (!stack.empty() && stack.back().type != Type::LeftParen) {
                    output.push_back(stack.back());
                    stack.pop_back();
                }
                if (stack.empty()) throw CalcError("parenthèse fermante sans ouvrante");
                stack.pop_back();
                if (!stack.empty() && stack.back().type == Type::Function) {
                    output.push_back(stack.back());
                    stack.pop_back();
                }
                break;
        }
    }
    while (!stack.empty()) {
        if (stack.back().type == Type::LeftParen) throw CalcError("parenthèse ouvrante non fermée");
        if (stack.back().type == Type::Function) throw CalcError("appel de fonction incomplet");
        output.push_back(stack.back());
        stack.pop_back();
    }
    return output;
}

Calculator::Calculator() : vars_{{"pi", std::acos(-1.0)}, {"e", std::exp(1.0)}} {}

static double applyBinary(char op, double a, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw CalcError("division par zéro");
            return a / b;
        case '%':
            if (b == 0) throw CalcError("modulo par zéro");
            return std::fmod(a, b);
        case '^': return std::pow(a, b);
    }
    throw CalcError(std::string("opérateur inconnu : ") + op);
}

double Calculator::evalRpn(const std::vector<Token>& rpn) const {
    std::vector<double> st;
    auto pop = [&st]() {
        if (st.empty()) throw CalcError("expression incomplète");
        double v = st.back();
        st.pop_back();
        return v;
    };

    for (const Token& t : rpn) {
        if (t.type == Type::Number) {
            st.push_back(t.value);
        } else if (t.type == Type::Variable) {
            auto it = vars_.find(t.text);
            if (it == vars_.end()) throw CalcError("variable inconnue : " + t.text);
            st.push_back(it->second);
        } else if (t.type == Type::Function) {
            auto it = kFunctions.find(t.text);
            if (it == kFunctions.end()) throw CalcError("fonction inconnue : " + t.text);
            st.push_back(it->second(pop()));
        } else if (t.text == "neg") {
            st.push_back(-pop());
        } else {
            double b = pop(), a = pop();
            st.push_back(applyBinary(t.text[0], a, b));
        }
    }
    if (st.size() != 1) throw CalcError(st.empty() ? "expression vide" : "opérateur manquant");
    return st.back();
}

double Calculator::evaluate(const std::string& line) {
    std::size_t eq = line.find('=');
    if (eq == std::string::npos) return evalRpn(toRpn(tokenize(line)));

    auto lhs = tokenize(line.substr(0, eq));
    if (lhs.size() != 1 || lhs[0].type != Type::Variable) throw CalcError("affectation invalide (attendu : nom = expression)");
    if (lhs[0].text == "pi" || lhs[0].text == "e") throw CalcError("impossible de modifier une constante");
    double value = evalRpn(toRpn(tokenize(line.substr(eq + 1))));
    vars_[lhs[0].text] = value;
    return value;
}
