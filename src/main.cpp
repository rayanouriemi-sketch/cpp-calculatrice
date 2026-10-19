#include <iostream>
#include <string>

#include "calculator.hpp"

int main() {
    Calculator calc;
    std::string line;
    std::cout << "Calculatrice - tapez une expression, \"vars\" pour les variables, \"quit\" pour quitter.\n";
    while (std::cout << "> " << std::flush, std::getline(std::cin, line)) {
        if (line == "quit" || line == "exit") break;
        if (line.find_first_not_of(" \t") == std::string::npos) continue;
        if (line == "vars") {
            for (const auto& [name, value] : calc.variables()) std::cout << "  " << name << " = " << value << '\n';
            continue;
        }
        try {
            std::cout << calc.evaluate(line) << '\n';
        } catch (const CalcError& e) {
            std::cout << "Erreur : " << e.what() << '\n';
        }
    }
    return 0;
}
