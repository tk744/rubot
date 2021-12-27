#include "rank.h"

static int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    else {
        return n * factorial(n-1);
    }
}

static int permutation(int n, int k) {
    return (int)(((float)factorial(n)) / factorial(n-k));
}

static int combination(int n, int k) {
    if (n < 0 || k < 0 || k > n) {
        return 0;
    }
    int b, i;
    for (b=1, i=0; i<k ; b*=((float)n-i)/(i+1), i++);
    return b;
}

static int binaryOnes(int b) {
    int n = 0;
    while (b > 0) {
        n += b % 2;
        b /= 2;
    }
    return n;
}

int permutationRank(Int8 *x, int n, int k) {
    int rank = 0, bits = 0, i;
    for (i=0 ; i<n ; i++) {
        bits |= (1 << (n-1-x[i]));
        rank += permutation(n-1-i, k-1-i) * (x[i] - binaryOnes(bits >> (n-x[i])));
    }

    return rank;
}

int combinationRank(Int8 *x, int n, int k) {
    int rank, i;
    for (rank=combination(n, k), i=0 ; i<k ; rank -= combination(n-(x[i]+1), k-i), i++);
    return rank-1;
}

int combinationPairRank(Int8 *x, int n) {
    int rank = 0, pair_rank, i, j;
    Int8 exclusions[n-2];
    
    for (i=n ; i>2 ; i-=2) {
        // set variable base
        for (pair_rank=1, j=i; j>2 ; pair_rank*=combination(j-2, 2), j-=2);
        
        // get pair
        Int8 pair[2] = { x[n-i], x[n-i+1] };

        // update exclusions
        exclusions[n-i] = pair[0];
        exclusions[n-i+1] = pair[1];

        // reduce combination values if greater than excluded values
        Int8 pair_reduced[2] = { pair[0], pair[1] };
        for (j=0; j<n-i ; j++) {
            pair_reduced[0] -= exclusions[j] < pair[0] ? 1 : 0;
            pair_reduced[1] -= exclusions[j] < pair[1] ? 1 : 0;
        }

        // compute pair rank and updated total rank
        pair_rank *= combinationRank(pair_reduced, i, 2);
        rank += pair_rank;
    }
    return rank;
}