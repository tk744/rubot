# Overview

`rubot` is a blazing fast and user friendly Rubik's cube library written in C. It uses a highly compact 1MB lookup table to deliver instantations solutions with an average of 32 moves and a max of 46 moves.

Since `rubot` was originally designed for use in embedded systems, it implements [Thistletwaite's algorithm](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Thistlethwaite's_algorithm) which uses much smaller lookup tables compared to algorithms that produce even fewer moves.

# Installation

Build the executable `rubot` by running `make`.

# Usage

For a full list of commands, run `rubot -h`.

## Data Formats

`rubot` encodes a **move sequence** as a whitespace-separated list of moves described using [standard notation](https://ruwix.com/the-rubiks-cube/notation/). The six base moves are `U`, `L`, `F`, `R`, `B`, and `D`, representing a quarter clockwise turn on the corresponding face. Each base move can be suffixed by `i` to represent a counter-clockwise (i.e. inverse) turn, or `2` to represent a half turn.

`rubot` serializes cube states into **color strings**. A color string is 54 characters long, and is encoded as 6 substrings (one per face) of 9 characters (one per tile). The faces are arranged in the order U, L, F, R, B, D, and the tiles in each face are arranged in row-major order. It does not matter what characters are used for each color, as long as they are consistent. The figure below depicts the index of each tile in a color string:

<p align="center"><img alt="cube string encoding" src="cube-string.png" width="500"></p>

## Cube Solving

`rubot` can take a scrambled cube and return a solution sequence in one of two ways:

- From a color string.
    
    ```
    $ ./rubot PBBWWBRGGWRWPPRBBRGPPGGWYYPYWWGRPBRYRPGWBYBRRGGWYYBYYP
    Di Fi R Li U R L F R2 U2 F L R2 D2 R2 F U2 F R2 B2 D2 B U2 R2 U2 F2 R2 U2 F2 U2 L2 B2 L2 U2
    ```

- From a scramble sequence.
    
    ```
    $ ./rubot F L2 D F B L2 U2 R U2 F D2 Fi B U B
    U R F2 B D U2 R2 Bi Ri U2 Li Fi R F D2 F2 D2 R2 F R2 B R2 U2 F2 L2 F2 L2 B2 U2 L2
    ```

`rubot` can also print the state of the scrambled cube using one of two flags:

- `-c`: print the color string.
    
    ```
    $ ./rubot -c D2 L B D B U2 Fi L B D U D Fi Di R
    FBURURBFDUFRULBFUDDLFLFLBDDLDRURLFUUBRRDBFLBULFRRDBLDB
    ```

- `-d`: draw a colored ASCII representation (requires a terminal with ANSII escape codes).
    
    ```
    $ ./rubot -d PBBWWBRGGWRWPPRBBRGPPGGWYYPYWWGRPBRYRPGWBYBRRGGWYYBYYP
    <try it in a terminal>
    ```

## Cube Scrambling

`rubot` also provides functionality for generating random scramble sequences:

- Pass the number of moves to generate, optionally followed by an RNG seed.
    
    ```
    $ ./rubot 25 5
    D2 U Fi B D F Ui Fi Ui F2 D B Fi D2 U Ri Bi F R L2 B2 Di B Ui Bi
    ```

- As with cube solving, we can use the `-c` or `-d` flag to print the state of the scrambled cube.
    
    ```
    $ ./rubot -c 1000
    URBFULFDULLRULRLDBUBLFFBRBDFUDLRDRUDLDBFBFFUDURBLDBFRR
    ```

## Error Handling

`rubot` will print to stderr and return a non-zero value if the input is invalid or if the cube is unsolvable:

- Return -1 on invalid inputs with a descriptive error message.
    
    ```
    $ ./rubot UUUUUUUUULLLxLLLLLFFFFFFFFFRRRRRRRRRBBBBBBBBBDDDDDDDDD
    Error: Invalid color 'x' at index 12.
    ```

- Return 1 on unsolvable cubes, regardless of the operation.
    
    ```
    ./rubot -d DUUUUUUUULLLLLLLLLFFFFFFFFFRRRRRRRRRBBBBBBBBBDDDDDDDDU
    No solution found.
    ```

## Benchmarking

`rubot` can even run a performance benchmark by solving a large number of cubes using the `-b` flag:

- `-b N`: benchmark on `N` scrambled cubes (output from my i7-13700K).

    ```
    $ ./rubot -b 100000
    Throughput: 4585.70 solves per second
    Length: 32.19 moves per solve
    ```

<!-- from v1.0

# How It Works

*NOTE: This is a very high-level overview of a beautiful group theory algorithm, and really does not do it justice beyond setting up the motivation for implementation details. The interested reader should absolutely do further research and investigate the code for more information.*

This program implements [Thistletwaite's algorithm](https://en.wikipedia.org/wiki/Optimal_solutions_for_Rubik%27s_Cube#Thistlethwaite's_algorithm) to solve Rubik's cubes because it provides an optimal compromise between solution length and memory. The algorithm divides the solving process into four phases, each with an increasingly restrictive moveset to prevent destroying progress made in a previous phase. The state space of possible cubes in each phase is relatively small, so we can explore it once and store it in a lookup table. Then when presented with a new cube, we simply lookup the next move in the lookup table for each phase until we arrive at the solved cube.

- To achieve maximum performance, we encode cubes into 128-bits. This is critical because generating the lookup table requires exploring approximately 50 million cube states.

- To achieve maximum memory efficiency, we encode two enties per byte in the lookup table. This is possible because the depth cannot exceed 15 for any phase, so it can be encoded into 4 bits.

- other implementation details.
 -->
<!-- TODO: make pretty
# References

Here are some links I found useful while developing this program.

1. https://www.jaapsch.net/puzzles/thistle.htm

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite_cpp.txt

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite.txt

1. https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9

1. https://medium.com/@benjamin.botto/sequentially-indexing-permutations-a-linear-algorithm-for-computing-lexicographic-rank-a22220ffd6e3

1. https://stackoverflow.com/a/3143594/14043949

1. https://stackoverflow.com/a/66608800/14043949
 -->

<!-- From v0.1

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
G4 | 

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

<!--
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

 -->