# Rubot

Rubot is a blazing fast Rubik's cube solver written in C. It generates a tiny ~1 MB lookup table which can be searched in nanoseconds. Solutions average 32 moves, with a guaranteed max of 46.

Rubot implements [Thistletwaite's](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Thistlethwaite's_algorithm) algorithm, a major breakthrough which introduced a novel approach using group theory in 1981. This algorithm was selected over later approaches such as [Kociemba's](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Kociemba's_algorithm) algorithm and [Korf's](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Korf's_algorithm) algorithm which produce even fewer moves because those methods either take more time to solve or require significantly larger lookup tables.

This project started as a final project for ECE4670: Digital Systems Design Using Microcontrollers in Fall 2021.

## Usage

Build the executable `./rubot` using the provided `Makefile` by running:
```
make
```

The first way to use `./rubot` is to generate both a scramble and a solution. The number of moves in the scramble is passed to `./rubot`, then the sequence of scramble and solution moves is printed to the console. For example:
```
./rubot 25
```

The second way to use `./rubot` is to generate a solution for a given scramble. The sequence of scramble moves is passed to `./rubot`, then the sequence of solution moves is printed to the console. Remember to escape `'` on the console. For example:
```
./rubot B2, U2, L, D\', R\', L2, B, R, L\', U
```

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

1. [Thistlethwaite's 52-move Algorithm](https://www.jaapsch.net/puzzles/thistle.htm)

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite_cpp.txt

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite.txt

1. [Implementing an Optimal Rubik’s Cube Solver using Korf’s Algorithm](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9)

1. [Sequentially Indexing Permutations: A Linear Algorithm for Computing Lexicographic Rank](https://medium.com/@benjamin.botto/sequentially-indexing-permutations-a-linear-algorithm-for-computing-lexicographic-rank-a22220ffd6e3)

1. https://stackoverflow.com/a/3143594/14043949

1. https://stackoverflow.com/a/66608800/14043949

