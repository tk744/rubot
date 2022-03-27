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

A brute-force search over the space of all cube states would be infeasible. With a branching factor of 12 quarter-turn moves, and a [theoretical maximum depth of 20 moves](https://www.cube20.org/), the state-space contains up to 12^20 possible cubes. Thistletwaite's algorithm instead divides the search into sequential phases, each with smaller branching factor and depth to make searching feasible. 

Many algorithms divide the solving process into multiple stages, where each subsequent stage is defined by a more restrictive property (e.g. one layer solved, two layers solved, etc.). But algorithms that contain few enough sequences for a human to memorize often require many moves because they require destroying and rebuilding properties established in previous stages to make progress towards the next one. Thistletwaite's algorithm uses far fewer moves because it never destroys such properties once they are established. 

This is cleverly achieved by searching for a cube at each phase which can be solved without quarter-turns of opposite faces, and then solving subsequent phases without quarter-turns of those faces. Half-turns are still allowed on those faces because they preserve the no-quarter-turn property. Then the final phase can be solved using only half-turns. Reducing the moveset at each phase has two desirable outcomes: first, it gradually reduces the branching factor for each successive search; second, it prevents the property established at each phase from being destroyed in subsequent stages. These two outcomes make the algorithm feasible and efficient.

Thistletwaite uses group theory to describe the state-space of cubes in each phase. A group is defined by a set of moves, and contains the set of cubes that can be solved using only those moves. The group which contains all cubes is defined by the set of all moves, while the identity group which only contains the solved cube is defined by the set of no moves. Each phase of the algorithm performs a brute-force search for a progressively more restrictive subgroup until reaching the identity group. The 5 groups used by the algorithm are listed below:

group | move set | 
:- | :- |
G0 | U, D, R, L, F, B
G1 | U2, D2, R, L, F, B
G2 | U2, D2, R2, L2, F, B
G3 | U2, D2, R2, L2, F2, B2
G4 | -

In general it is not trivial to verify whether a given cube is contained within a particular group. However the groups used by Thistletwiate are equivalent to fixing the orientations and positions of certain edge and corner cubies. For example, if we define the orientation of edges to change on U and D quarter-turns, then restricting such turns fixes the orientation of all the edges. Therefore any cube in G1 must have the correct orientation on all its edges, which is trivial to verify.

### Implementation

#### Cube Model

A cubie is one of the 20 mini-cubes which rotate together on the actual cube. An arbitrary cubie state has a unique orientation and position. Edge cubies can have 1 of 2 orientations and 12 positions, which take 1 and 4 bits to encode respectively. Corner cubies can have 1 of 3 orientations and 8 positions, which take 2 and 3 bits respectively. The entire cube state is represented by 2 64-bit integers, one encoding all 12 5-bit edge states and the other all 8 5-bit corner states in arbitrary order. 

<!--

Moves are represented by 8-bit chars. The right 6 bits are a one-hot encoding of the face the move applies to, and the left 2 bits are flags encoding whether the move is  
-->

TODO: table generation

<!-- 

-->

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

