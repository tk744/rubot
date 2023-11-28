# Rubot

Rubot is a blazing fast Rubik's cube solver written in C. It generates a 1.05 MB lookup table to produce solutions in under 1 ms. Guarantees solutions with a max of 46 moves, with an average of 32 moves.

Rubot implements [Thistletwaite's](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Thistlethwaite's_algorithm) algorithm, a major breakthrough which introduced a novel approach using group theory in 1981. This algorithm was selected over later approaches such as [Kociemba's](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Kociemba's_algorithm) algorithm and [Korf's](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Korf's_algorithm) algorithm which produce even fewer moves because those methods either take more time to solve or require significantly larger lookup tables.

This project started as a final project for ECE4670: Digital Systems Design Using Microcontrollers in Fall 2021.

# Installation

Build the executable by running `make`.

# Usage

The program takes a cube state and returns a list of moves which solve the cube.
```
$./rubot PBBWWBRGGWRWPPRBBRGPPGGWYYPYWWGRPBRYRPGWBYBRRGGWYYBYYP
D' F' R L' U R L F R2 U2 F L R2 D2 R2 F U2 F R2 B2 D2 B U2 R2 U2 F2 R2 U2 F2 U2 L2 B2 L2 U2
```
The state is represented as a 54-character string: 6 segments (one per face) of 9 characters (one per tile). The faces are arranged in the order U, L, F, R, B, D, and the tiles in each face are arranged in row-major order. The program is flexible and does not specify a fixed set of characters to use for each color. The graphic below depicts the index of each tile in the string representation.

You can also optionally pass the following flags to `./rubot` to modify its behavior:
- `-h` or `--help`: prints the help screen
<!-- - `-v`: verbose output -->

## Database Generation

The first time `./rubot` is run it will generate a 1.05 MB database. This will take either minutes or seconds, depending on your hardware. The chart below shows how long each phase took on my Intel Core i7-13700K.

phase | database size | database updates | states generated | time
-: | -: | -: | -: | -: 
1 | 2048 | 6491 | 68464 | 0.02 s
2 | 1082565 | 3319998 | 25938961 | 14.3 s
3 | 352800 | 1965338 | 13069024 | 11.1 s
4 | 663552 | 1766474 | 8832372 | 7.0 s

# References

1. https://www.jaapsch.net/puzzles/thistle.htm

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite_cpp.txt

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite.txt

1. https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9

1. https://medium.com/@benjamin.botto/sequentially-indexing-permutations-a-linear-algorithm-for-computing-lexicographic-rank-a22220ffd6e3

1. https://stackoverflow.com/a/3143594/14043949

1. https://stackoverflow.com/a/66608800/14043949

