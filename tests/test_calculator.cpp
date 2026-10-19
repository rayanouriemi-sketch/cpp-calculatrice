#undef NDEBUG  // les assert doivent rester actifs même en Release
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#include "../src/calculator.hpp"

static Calculator calc;

static bool near(const std::string& expr, double expected) {
    double got = calc.evaluate(expr);
    if (std::abs(got - expected) < 1e-9) return true;
    std::cerr << expr << " = " << got << " (attendu " << expected << ")\n";
    return false;
}

static bool fails(const std::string& expr) {
    try { calc.evaluate(expr); } catch (const CalcError&) { return true; }
    std::cerr << "aucune erreur pour : " << expr << '\n';
    return false;
}

int main() {
    assert(near("1 + 2 * 3", 7));
    assert(near("(1 + 2) * 3", 9));
    assert(near("10 / 4", 2.5));
    assert(near("10 - 4 - 3", 3));          // associativité à gauche
    assert(near("2 ^ 3 ^ 2", 512));         // associativité à droite
    assert(near("-2 ^ 2", -4));
    assert(near("2 ^ -1", 0.5));
    assert(near("-(3 + 4) * -2", 14));
    assert(near("+5", 5));
    assert(near("7 % 3", 1));
    assert(near("sqrt(16) + abs(-3)", 7));
    assert(near("cos(0) + ln(e)", 2));
    assert(near("sin(pi / 2)", 1));
    assert(near("1.5e3 + .5", 1500.5));

    // variables
    assert(near("x = 3 * 4", 12));
    assert(near("x / 2 + x", 18));
    assert(near("rayon = 2", 2));
    assert(near("pi * rayon ^ 2", 4 * std::acos(-1.0)));

    // erreurs
    assert(fails("1 / 0"));
    assert(fails("5 % 0"));
    assert(fails("(1 + 2"));
    assert(fails("1 + 2)"));
    assert(fails("1 +"));
    assert(fails("2 3"));
    assert(fails("y + 1"));
    assert(fails("foo(2)"));
    assert(fails("sqrt(-1)"));
    assert(fails("3 $ 4"));
    assert(fails("pi = 3"));
    assert(fails("2 = 3"));
    assert(fails(""));

    std::cout << "Tous les tests passent.\n";
    return 0;
}
