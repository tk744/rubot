#include "rubot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int C_FLAG = 0;
static int D_FLAG = 0;

static int run_help(char *prog_name) {
    printf("Usage:\n");
    printf("    %s [-d|c] COLOR_STRING\n", prog_name);
    printf("    %s [-d|c] MOVES ...\n", prog_name);
    printf("    %s [-d|c] N [SEED]\n", prog_name);
    printf("    %s -b N\n", prog_name);
    printf("    %s -h\n", prog_name);
    printf("Arguments:\n");
    printf("    COLOR_STRING: 54-char color string\n");
    printf("    MOVES:        whitespace-separated scramble sequence\n");
    printf("    N:            number of random moves to generate\n");
    printf("    SEED:         random number generator seed\n");
    printf("Options:\n");
    printf("    -h: display this text\n");
    printf("    -d: draw scrambled cube\n");
    printf("    -c: print color string\n");
    printf("    -b: benchmark on N solves\n");
    return 0;
}

static int run_solve(Cube c) {
    Move ms[MAX_MOVES];
    int n = solve(c, ms);
    if (n == -1) {
        fprintf(stderr, "No solution found.\n");
        return 1;
    }

    if (D_FLAG) {
        drawCube(c);
    }
    else if (C_FLAG) {
        printCube(c);
    }
    else {
        printMoves(ms, n);
    }

    return 0;
}

static int run_scramble(int n) {
    Cube c = cubeSolved();
    Move ms[n];
    setRandomMoves(ms, n);
    c = applyMoves(c, ms, n);
    
    if (C_FLAG) {
        printCube(c);
    }
    else if (D_FLAG) {
        drawCube(c);
    }
    else {
        printMoves(ms, n);
    }
    
    return 0;
}

static int run_benchmark(int n) {
    int scramble = 100;
    Move ms[scramble];
    Cube c = cubeSolved();
    
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
    // 0 args
    if (argc == 1) {
        return run_help(argv[0]);
    }

    // flags
    if (*argv[1] == '-') {
        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
            return run_help(argv[0]);
        }
        if (!strcmp(argv[1], "-c")) {
            C_FLAG = 1;
            argc--;
            argv++;
        }
        else if (!strcmp(argv[1], "-d")) {
            D_FLAG = 1;
            argc--;
            argv++;
        }
        else if (!strcmp(argv[1], "-b")) {
            if (argc != 3) {
                fprintf(stderr, "Error: Expected 1 argument to %s, recieved %i.\n", argv[1], argc-2);
                return -1;
            }
            int n;
            if (!sscanf(argv[2], "%i", &n)) {
                fprintf(stderr, "Error: Invalid argument. Expected integer, recieved '%s'.\n", argv[2]);
                return -1;
            }
            run_benchmark(n);
            return 0;
        }
        else {
            fprintf(stderr, "Error: Invalid flag '%s'.\n", argv[1]);
            return -1;
        }
    }

    // scramble
    int n, arg1_len = strlen(argv[1]);
    if (sscanf(argv[1], "%i", &n) && n != 54) {
        if (argc == 2) {
            srand(time(NULL));
        }
        else if (argc == 3) {
            int seed;
            if (sscanf(argv[2], "%i", &seed)) {
                srand(seed);
            }
            else {
                fprintf(stderr, "Error: Invalid seed. Expected integer, recieved '%s'.\n", argv[2]);
                return -1;
            }
        }
        else {
            fprintf(stderr, "Error: Too many integer arguments. Expected 1 or 2, recieved %i.\n", argc-1);
            return -1;
        }
        return run_scramble(n);
    }

    // solve from color string
    if (argc == 2 && arg1_len > 2) {
        Cube c = cubeSolved();
        if (arg1_len != 54) {
            fprintf(stderr, "Error: Invalid color string length. Expected 54, recieved %i.\n", arg1_len);
            return -1;
        }

        int error_code;
        if (error_code = parseCubeStr(&c, argv[1])) {
            fprintf(stderr, "Error: Invalid color '%c' at index %d.\n", *(argv[1]+error_code-1), error_code-1);
            return -1;
        }
        return run_solve(c);
    }

    // solve from scramble sequence
    else {
        Cube c = cubeSolved();
        int i;
        while (--argc > 0) {
            Move m;
            if ((parseMoveStr(&m, *(++argv))) == -1) {
                fprintf(stderr, "Error: Invalid move '%s'.\n", *argv);
                return -1;
            }
            c = applyMove(c, m);
        }
        return run_solve(c);
    }
}