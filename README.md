# Rubot

Rubot is a blazing fast Rubik's cube solver written in C. Utilizing a highly efficient 1MB lookup table, it delivers instantations solutions with a max of 46 moves and an average of 32 moves.

# Installation

Build the executable by running `make`.

# Usage

The program takes a cube state and returns a list of moves which solve the cube.
```
$./rubot PBBWWBRGGWRWPPRBBRGPPGGWYYPYWWGRPBRYRPGWBYBRRGGWYYBYYP
D' F' R L' U R L F R2 U2 F L R2 D2 R2 F U2 F R2 B2 D2 B U2 R2 U2 F2 R2 U2 F2 U2 L2 B2 L2 U2
```
The state is represented as a 54-character string: 6 segments (one per face) of 9 characters (one per tile). The faces are arranged in the order U, L, F, R, B, D, and the tiles in each face are arranged in row-major order. The program is flexible and does not specify a fixed set of characters to use for each color. The graphic below depicts the index of each tile in the string representation.

<center><img alt="cube string encoding" src="cube-encoding.png" width="300"></center>

Alternatively the program also accepts the sequence of moves used to scramble the cube. It does not simply reverse this sequence to provide a solution.
```
$ ./rubot L D2 R L\' R\' B F D L\' U\'
U L D U2 F' B' R D2 R' U2 F2 R2 F2 B2 R2 F2 D2 F2 B2
```

The program can also generate a random sequence of non-redundant moves for scrambling by accepting the number of moves as an integer. It also accepts an optional second parameter to seed the RNG for deterministic results.

```
$ ./rubot 15 0
D' B' L' R B' F' D2 R U' R2 F B2 F2 D' L
```

Note that the program will generate a 1MB lookup table the first time it is run, which will take a few extra seconds (my Intel Core i7-13700K takes about 17.5 seconds).

<!-- TODO:
# Performance

 -->

# How It Works

This program implements [Thistletwaite's algorithm](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Thistlethwaite's_algorithm) because it provides the optimal compromise between speed and memory. The algorithm divides the solving process into four phases, each with an increasingly restrictive moveset to prevent destroying progress made in a previous phase. The state space of possible cubes in each phase is relatively small, so we can explore it once and store it in a lookup table. Then when presented with a new cube, we simply lookup the next move in the lookup table for each phase until we arrive at the solved cube. This is just a high-level overview of a beautiful group theory idea which should be researched further if interesting to the reader.

<!-- TODO:
- To achieve maximum performance, we encode cubes into 128-bits. This is critical because generating the lookup table requires exploring approximately 50 million cube states.

- To achieve maximum memory efficiency, we encode two enties per byte in the lookup table. This is possible because the depth cannot exceed 15 for any phase, so it can be encoded into 4 bits.

- other implementation details.
 -->

# References

Here are some links I found useful while developing this program.

1. https://www.jaapsch.net/puzzles/thistle.htm

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite_cpp.txt

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite.txt

1. https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9

1. https://medium.com/@benjamin.botto/sequentially-indexing-permutations-a-linear-algorithm-for-computing-lexicographic-rank-a22220ffd6e3

1. https://stackoverflow.com/a/3143594/14043949

1. https://stackoverflow.com/a/66608800/14043949

