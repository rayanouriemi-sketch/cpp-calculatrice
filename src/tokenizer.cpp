#include <cctype>

#include "calculator.hpp"

namespace {
bool isIdentStart(char c) { return std::isalpha(static_cast<unsigned char>(c)) || c == '_'; }
bool isIdentChar(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }
}  // namespace

std::vector<Token> tokenize(const std::string& expr) {
    using Type = Token::Type;
    std::vector<Token> tokens;
    std::size_t i = 0;
    while (i < expr.size()) {
        char c = expr[i];
        if (std::isspace(static_cast<unsigned char>(c))) { ++i; continue; }

        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            std::size_t used = 0;
            double v;
            try { v = std::stod(expr.substr(i), &used); }
            catch (const std::exception&) { throw CalcError("nombre invalide à la position " + std::to_string(i + 1)); }
            tokens.push_back({Type::Number, expr.substr(i, used), v});
            i += used;
        } else if (isIdentStart(c)) {
            std::size_t start = i;
            while (i < expr.size() && isIdentChar(expr[i])) ++i;
            std::string name = expr.substr(start, i - start);
            std::size_t next = expr.find_first_not_of(" \t", i);
            bool isCall = next != std::string::npos && expr[next] == '(';
            tokens.push_back({isCall ? Type::Function : Type::Variable, name});
        } else if (c == '(') {
            tokens.push_back({Type::LeftParen, "("});
            ++i;
        } else if (c == ')') {
            tokens.push_back({Type::RightParen, ")"});
            ++i;
        } else if (std::string("+-*/%^").find(c) != std::string::npos) {
            // Un signe est unaire en début d'expression ou après un opérateur / une parenthèse ouvrante.
            bool unary = (c == '-' || c == '+')
                         && (tokens.empty() || tokens.back().type == Type::Operator
                             || tokens.back().type == Type::LeftParen);
            if (unary && c == '+') { ++i; continue; }  // le + unaire ne fait rien
            tokens.push_back({Type::Operator, unary ? "neg" : std::string(1, c)});
            ++i;
        } else {
            throw CalcError(std::string("caractère inattendu (") + c + ") à la position " + std::to_string(i + 1));
        }
    }
    return tokens;
}
