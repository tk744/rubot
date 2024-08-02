#include "rubot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // 0 args: help
    if (argc == 1 || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
        printf("Usage: %s [n <s>|ms|cs]\n", argv[0]);
        printf("    n: (int) get n random moves, optionally seed with s\n");
        printf("   ms: (str) whitespace-seperated list of moves\n");
        printf("   cs: (str) 54-length string of colors\n");
        return 0;
    }

    // 1-2 int args: get shuffled moves
    int n, seed;
    if (sscanf(argv[1], "%i", &n)) {
        if (argc == 2) {
            srand(time(NULL));
        }
        else if (argc == 3) {
            if (sscanf(argv[2], "%i", &seed)) {
                srand(seed);
            }
            else {
                printf("ERROR: Invalid seed. Expected integer, recieved '%s'.\n", argv[2]);
                return -1;
            }
        }
        else {
            printf("ERROR: Too many integer arguments. Expected 1 or 2, recieved %i.\n", argc-1);
            return -1;
        }

        Move ms[n];
        setRandomMoves(ms, n);
        printMoves(ms, n);
        return 0;
    }

    // initialize cube state from args
    Cube128 c = cubeSolved();

    // 1 string arg: set colors
    int arg1_len = strlen(argv[1]);
    if (argc == 2 && arg1_len != 1 && arg1_len != 2) {
        if (arg1_len != 54) {
            printf("ERROR: Invalid color string length. Expected 54, recieved %i.\n", arg1_len);
            return -1;
        }

        int idx_error = parseCube(&c, argv[1]);
        if (idx_error) {
            printf("ERROR: Invalid color '%c' in position %d.\n", *(argv[1]+idx_error), idx_error);
            return -1;
        }
    }

    // 1+ string args: apply move sequence
    else {
        while (--argc > 0) {
            Move m;
            if ((parseMove(&m, *(++argv))) == -1) {
                printf("ERROR: Invalid move '%s'.\n", *argv);
                return -1;
            }
            c = applyMove(c, m);
        }
    }

    // solve cube
    Move ms[MAX_MOVES];
    int l = solve(c, ms);
    if (l == -1) {
        printf("Invalid cube\n");
        return 1;
    }
    printMoves(ms, l);
    return 0;
}