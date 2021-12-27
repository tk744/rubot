# Rubot

Rubot is a blazing fast Rubik's cube solver written in C. It was designed to minimize computation time and memory usage for robots controlled by a microcontroller. This project started as a final project for ECE4670: Digital Systems Design Using Microcontrollers in Fall 2021.

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

The first time `./rubot` is run it will generate an approximately 1.05 MB database, which may take a few minutes. The database is only generated once and is stored in a file for subsequent use.

## How It Works

Rubot solves Rubik's cubes using [Thistletwaite's algorithm](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Thistlethwaite's_algorithm). This algorithm published in 1981 was a major breakthrough at the time of its conception because it could solve the Rubik's cube in fewer moves than any previous method by introducing a new approach based on group theory and computer searches. We chose this algorithm over alternatives that solve the Rubik's cube in even fewer moves like [Kociemba's algorithm](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Kociemba's_algorithm) and [Korf's algorithm](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Korf's_algorithm) because those methods either take more time to solve or require significantly larger lookup tables.

### Thistletwaite's Algorithm

<!-- Rubik's cube solving algorithms are generally divided into multiple stages, where each stage is defined by some property (e.g. one layer solved) and stages are solved sequentially. But algorithms designed for humans to memorize are inherently inefficient because they all require methodically destroying and rebuilding previous stages as progress is made towards the next stage. Thistletwaite's algorithm is efficient because it was designed so that once a stage is reached, it will never be destroyed in making progress towards subsequent stages.

Each stage in Thistletwaite's algorithm is described as a group using group theory. Each group is defined by a set of moves, and contains the set of cubes that can be solved using the moves defined in that group. The first group is defined by the set of all moves and therefore contains every possible cube, and the last group is defined by the empty set and therefore contains only the solved cube. -->

<!-- <hr> -->

<!-- It is clear that the last group is a subset of the first group.

Then each phase of the algorithm brings a cube from one group to the next until eventually reaching the identity group which only contains the solved cube. The five groups are described below:

group | move set | 
:- | :- |
G0 | U, D, R, L, F, B
G1 | U2, D2, R, L, F, B
G2 | U2, D2, R2, L2, F, B
G3 | U2, D2, R2, L2, F2, B2
G4 | - -->

TODO: explanation

### Implementation

TODO: cube model

TODO: table generation

TODO: table lookup

## Performance

All benchmarks were performed on my 2013 MacBook Pro with a 2.7 GHz Intel Core i5. In the future I will create a more consistent benchmarking system.

### Solution Length

I generated solutions to 1 million cubes randomly scrambled with 100 moves each to sample the distribution of solution lengths. The result was a mean of 32.2 moves and standard deviation of 2.4 moves.
<!-- 
solution length | occurances | probability
-: | -: | -:
18 | 2 | 0.000%
19 | 9 | 0.001%
20 | 28 | 0.003%
21 | 53 | 0.005%
22 | 171 | 0.017%
23 | 515 | 0.051%
24 | 1439 | 0.144%
25 | 3595 | 0.359%
26 | 8629 | 0.863%
27 | 19017 | 1.902%
28 | 37087 | 3.709%
29 | 65201 | 6.520%
30 | 100729 | 10.073%
31 | 136138 | 13.614%
32 | 159226 | 15.923%
33 | 160464 | 16.046%
34 | 135301 | 13.530%
35 | 93196 | 9.320%
36 | 50799 | 5.080%
37 | 20718 | 2.072%
38 | 6152 | 0.615%
39 | 1276 | 0.128%
40 | 227 | 0.023%
41 | 27 | 0.003%
42 | 1 | 0.000% 
-->

### Database Generation

phase | database size | database updates | states generated | time
-: | -: | -: | -: | -: 
1 | 2048 | 6491 | 68464 | < 0.2 s
2 | 1082565 | 3319998 | 25938961 | < 90 s
3 | 352800 | 1965338 | 13069024 | < 60 s
4 | 663552 | 1766474 | 8832372 | < 45 s

## References

1. [Thistlethwaite's 52-move Algorithm](https://www.jaapsch.net/puzzles/thistle.htm)

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite_cpp.txt

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite.txt

1. [Implementing an Optimal Rubik’s Cube Solver using Korf’s Algorithm](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9)

1. [Sequentially Indexing Permutations: A Linear Algorithm for Computing Lexicographic Rank](https://medium.com/@benjamin.botto/sequentially-indexing-permutations-a-linear-algorithm-for-computing-lexicographic-rank-a22220ffd6e3)

1. https://stackoverflow.com/a/3143594/14043949

1. https://stackoverflow.com/a/66608800/14043949

