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