// #include "solver.h"
// #include <solver.h>

// void analysis1() {
//     srand(&analysis1);

//     // hyperparameters
//     int n = 100, s = 1000000;
//     printf("n = %d, s = %d\n", n, s);

//     // initialize data
//     int i;
//     int ds[MAX_MOVES+1];
//     for (i=0 ; i<MAX_MOVES+1 ; i++) {
//         ds[i] = 0;
//     }

//     // generate data
//     Move ms_scramble[n], ms_solved[MAX_MOVES];
//     for (i=0 ; i<s; i++) {
//         int m = solve(scramble(solvedCube(), ms_scramble, n), ms_solved);
//         ds[m]++;
//     }

//     // print data
//     for (i=0 ; i<=MAX_MOVES ; i++) {
//         printf("Depth %2u | %4u : %2.3f\n", i, ds[i], 100.0*ds[i]/s);
//     }

//     // compute statistics
//     double mean = 0;
//     for (i=0 ; i<=MAX_MOVES ; i++) {
//         mean += i * ds[i];
//     }
//     mean /= s;

//     double var = 0;
//     for (i=0 ; i<=MAX_MOVES ; i++) {
//         var += ((i - mean) * (i - mean)) * ds[i];
//     }
//     var /= s;

//     // print statistics
//     printf("\nmean: %3.3f, variance: %3.3f\n", mean, var);
// }

// int main() {
//     analysis1();
//     return 0;
// }

// #include "color.c"
// #include <stdio.h>

// int main() {
//     int n = 1524; // todo - shouldn't have to write 10 twice
//     Move ms_scramble[n];
//     Cube128 c1 = scramble(solvedCube(), ms_scramble, n);
//     ColorCube cc = convertCube(c1);
//     Cube128 c2 = convertColorCube(cc);
    


//     printCube(c1);
//     printColorCube(cc);
//     printCube(c2);

// }