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

    printf("Throughput: %.2f solves per second\n", n / total_time);
    printf("Length: %.2f moves per solve\n", (double)total_length / n);
}

int main(int argc, char *argv[]) {
    // 0 args or -h/--help
    if (argc == 1 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        printf("Usage:\n");
        printf("    %s [-d|c] COLOR_STRING\n", argv[0]);
        printf("    %s [-d|c] MOVES ...\n", argv[0]);
        printf("    %s [-d|c] N [SEED]\n", argv[0]);
        printf("    %s -b N\n", argv[0]);
        printf("    %s -h\n", argv[0]);
        // printf("\n");
        printf("Arguments:\n");
        printf("    COLOR_STRING: 54-length color string\n");
        printf("    MOVES:        scramble move sequence\n");
        printf("    N:            number of random moves to generate\n");
        printf("    SEED:         random number generator seed\n");
        // printf("\n");
        printf("Options:\n");
        printf("    -h: display this text\n");
        printf("    -d: draw scrambled cube\n");
        printf("    -c: print color string\n");
        printf("    -b: benchmark on N solves\n");
        return 0;
    }

    // flags
    int c_flag = 0, d_flag = 0;
    if (*argv[1] == '-') {
        // -b: benchmark
        if (!strcmp(argv[1], "-b")) {
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
        // -c
        else if (!strcmp(argv[1], "-c")) {
            c_flag = 1;
            argc--;
            argv++;
        }
        // -d
        else if (!strcmp(argv[1], "-d")) {
            d_flag = 1;
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
        c = applyMoves(c, ms, n);
        
        if (d_flag) {
            drawCube(c);
        }
        else if (c_flag) {
            printCube(c);
        }
        else {
            printMoves(ms, n);
        }

        return 0;
    }

    // 1 string arg: set colors
    int arg1_len = strlen(argv[1]);
    if (argc == 2 && arg1_len != 1 && arg1_len != 2) {
        if (arg1_len != 54) {
            printf("ERROR: Invalid color string length. Expected 54, recieved %i.\n", arg1_len);
            return -1;
        }

        int error_code;
        if (error_code = parseCubeStr(&c, argv[1])) {
            printf("ERROR: Invalid color '%c' at index %d.\n", *(argv[1]+error_code-1), error_code-1);
            return -1;
        }
    }

    // 1+ string args: apply move sequence
    else {
        while (--argc > 0) {
            Move m;
            if ((parseMoveStr(&m, *(++argv))) == -1) {
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

    if (d_flag) {
        drawCube(c);
    }
    else if (c_flag) {
        printCube(c);
    }
    else {
        printMoves(ms, l);
    }

    return 0;
}