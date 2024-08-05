# Overview

`rubot` is a blazing fast and user friendly Rubik's cube solver written in C. It uses a highly compact 1MB lookup table to deliver instantations solutions with an average of 32 moves and a max of 46 moves.

Since `rubot` was originally designed for use in embedded systems, it implements [Thistletwaite's algorithm](https://www.jaapsch.net/puzzles/thistle.htm) which uses much smaller lookup tables compared to algorithms that produce even fewer moves.

# Installation

### Linux

- Install (default location is `/usr/local/bin`):
    ```
    git clone https://github.com/tk744/rubot
    cd rubot
    sudo make install
    ```

- Uninstall:

    ```
    sudo make uninstall
    ```

### Build from Source

Build the executable `rubot` from source by running `make`.

# Usage

For a full list of commands, run `rubot -h`.

## Encoding

A scramble or solution is encoded as a **move sequence**:
- A move sequence is a whitespace-separated list of moves described using [standard notation](https://ruwix.com/the-rubiks-cube/notation/). The six base moves are `U`, `L`, `F`, `R`, `B`, `D`, which can be suffixed by `i` and `2` for inverse and half-turns.

A cube is serialized as a **color string**:
- A color string has 54 chars, one for each tile. It is arranged linearly by face, with the tiles in each face arranged in row-major order, and the faces arranged U, L, F, R, B, D. Colors are represented by any set of 6 unique chars.

<p align="center">
    <img 
    src="cube-string.png"
    alt="color string representation of a cube" 
    width="400">
    <br>
    <i>The index of each tile in the color string representation of a cube.</i>
</p>

## Cube Solving

`rubot` can take a scrambled cube and return a solution sequence in one of two ways:

- From a color string.
    ```
    $ ./rubot LBBUUBRFFURULLRBBRFLLFFUDDLDUUFRLBRDRLFUBDBRRFFUDDBDDL
    Di Fi R Li U R L F R2 U2 F L R2 D2 R2 F U2 F R2 B2 D2 B U2 R2 U2 F2 R2 U2 F2 U2 L2 B2 L2 U2
    ```

- From a scramble sequence.
    
    ```
    $ ./rubot F L2 D F B L2 U2 R U2 F D2 Fi B U B
    U R F2 B D U2 R2 Bi Ri U2 Li Fi R F D2 F2 D2 R2 F R2 B R2 U2 F2 L2 F2 L2 B2 U2 L2
    ```

`rubot` can also print the state of the scrambled cube using either the `-c` or `-d` flag:

- `-c`: print the color string.
    
    ```
    $ ./rubot -c D2 L B D B U2 Fi L B D U D Fi Di R
    FBURURBFDUFRULBFUDDLFLFLBDDLDRURLFUUBRRDBFLBULFRRDBLDB
    ```

- `-d`: draw a colored ASCII representation (requires a terminal with ANSII escape codes).
    
    ```
    $ ./rubot -d UDUDUDUDULRLRLRLRLFBFBFBFBFRLRLRLRLRBFBFBFBFBDUDUDUDUD
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

## Benchmarking

`rubot` can even run a simple benchmark by solving a large number of cubes using the `-b` flag:

- `-b N`: benchmark on `N` scrambled cubes (output from my i7-13700K).

    ```
    $ ./rubot -b 100000
    Throughput: 4585.70 solves per second
    Length: 32.19 moves per solve
    ```

## Input Validation

`rubot` will print to stderr and return a non-zero value if the input is invalid or if the cube is unsolvable:

- Return -1 on invalid inputs with a descriptive error message.
    
    ```
    $ ./rubot UUUUUUUUULLLxLLLLLFFFFFFFFFRRRRRRRRRBBBBBBBBBDDDDDDDDD
    Error: Invalid color 'x' at index 12.
    ```

- Return 1 on unsolvable cubes, regardless of the operation.
    
    ```
    $ ./rubot -d DUUUUUUUULLLLLLLLLFFFFFFFFFRRRRRRRRRBBBBBBBBBDDDDDDDDU
    No solution found.
    ```

<!-- 
# References

1. [Thistlethwaite's 52-move Algorithm](https://www.jaapsch.net/puzzles/thistle.htm)

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite_cpp.txt

1. https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite.txt

1. [Implementing an Optimal Rubik’s Cube Solver using Korf’s Algorithm](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9)

1. [Sequentially Indexing Permutations: A Linear Algorithm for Computing Lexicographic Rank](https://medium.com/@benjamin.botto/sequentially-indexing-permutations-a-linear-algorithm-for-computing-lexicographic-rank-a22220ffd6e3)

1. https://stackoverflow.com/a/3143594/14043949

1. https://stackoverflow.com/a/66608800/14043949  -->
