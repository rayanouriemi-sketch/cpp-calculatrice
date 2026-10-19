# Calculatrice scientifique en C++

Un interpréteur d'expressions mathématiques en ligne de commande, écrit sans aucune bibliothèque de parsing.

## Fonctionnalités

- Opérateurs `+ - * / % ^` avec les bonnes priorités et associativités (`2^3^2 = 512`)
- Moins unaire (`-2^2 = -4`, `2^-1 = 0.5`)
- Parenthèses
- Fonctions : `sqrt`, `abs`, `sin`, `cos`, `tan`, `ln`, `exp`
- Constantes `pi` et `e`, variables (`x = 3 * 4`)
- Messages d'erreur clairs : division par zéro, parenthèses mal fermées, variable inconnue...

## Exemple

```
> rayon = 3
3
> pi * rayon ^ 2
28.2743
> sqrt(16) + abs(-3)
7
> (1 + 2
Erreur : parenthèse ouvrante non fermée
```

## Fonctionnement

1. **Analyse lexicale** (`tokenizer.cpp`) : le texte est découpé en jetons (nombres, opérateurs, noms...).
2. **Shunting-yard** (algorithme de Dijkstra) : conversion en notation polonaise inverse (`1 + 2 * 3` devient `1 2 3 * +`).
3. **Évaluation** avec une pile.

## Compiler et tester

```bash
cmake -B build && cmake --build build
./build/calc
ctest --test-dir build --output-on-failure
```
