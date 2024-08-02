#include "rubot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int benchmark(int n) {
    int scramble = 100;
    Move ms[scramble];
    Cube128 c = cubeSolved();
    
    double total_time = 0;
    int total_length = 0;
    int i;
    for (i=0 ; i < n ; i++) {
        setRandomMoves(ms, scramble);
        c = applyMoves(c, ms, scramble);

        clock_t clock_start = clock();
        total_length += solve(c, ms);
        total_time += (double)(clock() - clock_start) / CLOCKS_PER_SEC;
    }

    printf("Throughput: %.2f cubes per second\n", n / total_time);
    printf(" Solutions: %.2f moves per cube\n", (double)total_length / n);
}

int main(int argc, char *argv[]) {
    // 0 args or -h: help
    if (argc == 1 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        printf("Usage: %s [n <s>|ms|cs]\n", argv[0]);
        printf("    n: (int) get n random moves, optionally seed with s\n");
        printf("   ms: (str) whitespace-seperated list of moves\n");
        printf("   cs: (str) 54-length string of colors\n");
        return 0;
    }

    // flags
    int print_flag = 0;
    if (*argv[1] == '-') {
        // -b: benchmark
        if (!strcmp(argv[1], "-b") || !strcmp(argv[1], "--benchmark")) {
            if (argc != 3) {
                printf("ERROR: Expected 1 argument to %s, recieved %i.\n", argv[1], argc-2);
                return -1;
            }
            int n;
            if (!sscanf(argv[2], "%i", &n)) {
                printf("ERROR: Invalid argument. Expected integer, recieved '%s'.\n", argv[2]);
                return -1;
            }
            benchmark(n);
            return 0;
        }
        // -p: print cube
        else if (!strcmp(argv[1], "-p") || !strcmp(argv[1], "--print")) {
            print_flag = 1;
            argc--;
            argv++;
        }
        else {
            printf("ERROR: Invalid flag '%s'.\n", argv[1]);
            return -1;
        }
    }

    // cube for printing or solving
    Cube128 c = cubeSolved();

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
        if (print_flag) {
            c = applyMoves(c, ms, n);
            printCube(c);
        }
        printMoves(ms, n);
        return 0;
    }

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
    else if (print_flag) {
        printCube(c);
    }
    printMoves(ms, l);
    return 0;
}