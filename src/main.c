#include "rubot.h"
#include <stdio.h>

static Move parseMove(char *str) {
    Move m = NOP;
    
    int i;
    for (i=0; *str != '\0'; i++, str++) {
        // either U, D, F, B, R, L
        if (i == 0) {
            if (*str == 'U') {
                m |= U;
            }
            else if (*str == 'D') {
                m |= D;
            }
            else if (*str == 'F') {
                m |= F;
            }
            else if (*str == 'B') {
                m |= B;
            }
            else if (*str == 'R') {
                m |= R;
            }
            else if (*str == 'L') {
                m |= L;
            }
            else {
                m = NOP;
                break;
            }
        }
        // either ', 2
        else if (i == 1) {
            if (*str == '\'') {
                m |= I;
            }
            else if (*str == '2') {
                m |= H;
            }
            else if (*str != ',') {
                m = NOP;
                break;
            }
        }
        // error if not seperator
        else if (*str != ',') {
            m = NOP;
            break;
        }
    }

    return m;
}

static Cube128 parseInput(int argc, char *argv[]) {
    Cube128 c = cubeSolved();

    // no passed args
    if (argc == 1) {
        return c;
    }

    // help
    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-help")) {
        printf("Usage: %s [n|ms]\n", argv[0]);
        printf("    n: (int) number of random scramble moves\n");
        printf("   ms: (str) sequence of scramble moves\n");
        return c;
    }

    // passed int
    int n;
    if (sscanf(argv[1], "%i", &n) == 1) {
        Move ms[n];
        setRandomMoves(ms, n);
        printMoves(ms, n);
        c = applyMoves(c, ms, n);
    }
    // passed move sequence
    else {
        while (--argc > 0) {
            Move m = parseMove(*(++argv));
            if (m == NOP) {
                printf("Skipping invalid move: %s\n", *argv);
            }
            else {
                c = applyMove(c, m);
            }
        }
    }

    return c;
}

int main(int argc, char *argv[]) {
    // get cube state from arguments
    Cube128 c = parseInput(argc, argv);

    // print cube state
    printf("\nINITIAL STATE:\n");
    printCube(c);

    // find solution move sequence
    Move ms[MAX_MOVES];
    int n = solve(c, ms);

    // print solution move sequence
    printf("\nSOLUTION:\n");
    printMoves(ms, n);

    printf("\n");
    c = applyMoves(c, ms, n);
    printCube(c);

    return 0;
}