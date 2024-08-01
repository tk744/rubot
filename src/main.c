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

    // 1 string arg: get solution moves from initial colors
    int arg1_len = strlen(argv[1]);
    if (argc == 2 && arg1_len != 1 && arg1_len != 2) {
        if (arg1_len == 54) {
            Cube128 c = cubeSolved();
            if (parseCube(&c, argv[1])) {
                Move ms[MAX_MOVES];
                int n = solve(c, ms);
                printMoves(ms, n);
            }
            else {
                printf("ERROR: Invalid color string '%s'.\n", argv[1]);
                return -1;
            }
        }
        else {
            printf("ERROR: Invalid color string length. Expected 54, recieved %i.\n", arg1_len);
            return -1;
        }
        return 0;
    }

    // 1+ string args: get solution moves from initial move
    if (argc > 1) {
        Cube128 c = cubeSolved();
        Move m;
        while (--argc > 0) {
            if ((parseMove(&m, *(++argv))) == -1) {
                printf("ERROR: Invalid move '%s'.\n", *argv);
                return -1;
            }
            else {
                c = applyMove(c, m);
            }
        }
        Move ms[MAX_MOVES];
        int n = solve(c, ms);
        printMoves(ms, n);
        return 0;
    }
}