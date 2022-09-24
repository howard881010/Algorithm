// EE3980 HW10 Coin Set Design, II
// 107061240,
// 2021/5/22

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int **g;             // To record g[n][D]
int *table;          // To store the minimum number of coins D

int NCoinDP_R(int D, int Ncoin, int Coins[]);   // DP recursive approach
int NCoinDP_TD(int D, int Ncoin, int Coins[]);  // DP top-down approach
void NCoinDP_BU(int D, int Ncoin, int Coins[]); // DP bottom approach
double getTime(void);                           // Get the local time

int main(void)
{
    int i, j, k, l;                       // Loop index
    int Coins[4] = {1, 5, 10, 50};        // Original coin set
    int num;                              // Total number of coins
    int min;                              // Minimum total number of coins
    int x, y;                             // Record variable
    double t;                             // Record time

    // Allocate space for g and table
    g = (int **)malloc(4 * sizeof(int *));
    table = (int *)malloc(100 * sizeof(int));
    for (i = 0; i < 4; i++) {
        g[i] = (int *)malloc(100 * sizeof(int));
    }
    // For coin set {1, 5, 10, 50}
    printf("Original coin set:\n");
    // For DP recursive
    t = getTime();
    num = 0;
    // Determines the minimum number of coins to represent a integer i
    for (i = 1; i < 100; i++) {
        num += NCoinDP_R(i, 4, Coins);
    }
    t = getTime() - t;
    // Print the result
    printf("  DP recursive: {1, 5, 10, 50} average is %.5f", (float)num / 99);
    printf(", CPU time: %g sec\n", t);
    // For DP top-down
    t = getTime();
    // Initialize the g array
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 100; j++) {
            g[i][j] = 0;
            g[0][j] = j;           // g[0][D] = D
        }
    }
    // Determines the minimum number of coins to represent a integer i
    num = 0;
    for (i = 1; i < 100; i++) {
        num += NCoinDP_TD(i, 4, Coins);
    }
    t = getTime() - t;
    // Print the result
    printf("  DP top-down:  {1, 5, 10, 50} average is %.5f", (float)num / 99);
    printf(", CPU time: %g sec\n", t);
    // For bottom-up
    t = getTime();
    num = 0;
    // Determines the minimum number of coins to represent a integer 99
    NCoinDP_BU(99, 4, Coins);
    for (i = 1; i < 100; i++) {
        num += table[i];
    }
    t = getTime() - t;
    printf("  DP bottom-up: {1, 5, 10, 50} average is %.5f", (float)num / 99);
    printf(", CPU time: %g sec\n", t);

    // Replacing $50:
    printf("Replacing $50:\n  ");
    // For DP recursive
    min = 10000;
    t = getTime();
    for (i = 11; i < 100; i++) {
        num = 0;
        Coins[3] = i;                       // For the coin set {1, 5, 10, i}
        // Determines the minimum number of coins to represent a integer j
        for (j = 1; j < 100; j++) {
            num += NCoinDP_R(j, 4, Coins);
        }
        if (num < min) {
            min = num;          // Record min
            x = i;              // Record x
        }
    }
    t = getTime() - t;
    // Print the result
    printf("DP recursive: {1, 5, 10, %d} average is %.5f", x, (float)min / 99);
    printf(", CPU time: %g sec\n  ", t);
    // For DP top-down
    t = getTime();
    min = 10000;
    for (i = 11; i < 100; i++) {
        // Initialize the g array
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 100; k++) {
                g[j][k] = 0;
                g[0][k] = k;           // g[0][D] = D
            }
        }
        num = 0;
        Coins[3] = i;                  // For the coin set {1, 5, 10, i}
        // Determines the minimum number of coins to represent a integer j
        for (j = 1; j < 100; j++) {
            num += NCoinDP_TD(j, 4, Coins);
        }
        if (num < min) {
            min = num;             // Record min
            x = i;                 // Record x
        }
    }
    t = getTime() - t;
    // Print the result
    printf("DP top-down:  {1, 5, 10, %d} average is %.5f", x, (float)min / 99);
    printf(", CPU time: %g sec\n  ", t);
    // For DP bottom-up
    t = getTime();
    min = 10000;
    for (i = 11; i < 100; i++) {
        num = 0;
        Coins[3] = i;                   // For the coin set {1, 5, 10, i}
        // Determines the minimum number of coins to represent a integer 99
        NCoinDP_BU(99, 4, Coins);
        for (j = 1; j < 100; j++) {
            num += table[j];
        }
        if (num < min) {
            min = num;           // Record min
            x = i;               // Record x
        }
    }
    t = getTime() - t;
    printf("DP bottom-up: {1, 5, 10, %d} average is %.5f", x, (float)min / 99);
    printf(", CPU time: %g sec\n", t);

    // Replacing $10:
    printf("Replacing $10:\n  ");
    Coins[3] = 50;
    // For DP recursive
    t = getTime();
    min = 10000;
    for (i = 6; i < 50; i++) {
        num = 0;
        Coins[2] = i;                    // For the coin set {1, 5, i, 50}
        // Determines the minimum number of coins to represent a integer j
        for (j = 1; j < 100; j++) {
            num += NCoinDP_R(j, 4, Coins);
        }
        if (num < min) {
            min = num;        // Record min
            x = i;            // Record x
        }
    }
    t = getTime() - t;
    // Print the result
    printf("DP recursive: {1, 5, %d, 50} average is %.5f", x, (float)min / 99);
    printf(", CPU time: %g sec\n  ", t);
    // For DP top-down
    t = getTime();
    min = 10000;
    for (i = 6; i < 50; i++) {
        // Initial the g array
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 100; k++) {
                g[j][k] = 0;
                g[0][k] = k;            // g[0][D] = D
            }
        }
        num = 0;
        Coins[2] = i;                   // For the coin set {1, 5, i, 50}
        // Determines the minimum number of coins to represent a integer j
        for (j = 1; j < 100; j++) {
            num += NCoinDP_TD(j, 4, Coins);
        }
        if (num < min) {
            min = num;              // Record min
            x = i;                  // Record x
        }
    }
    t = getTime() - t;
    // Print the result
    printf("DP top-down:  {1, 5, %d, 50} average is %.5f", x, (float)min / 99);
    printf(", CPU time: %g sec\n  ", t);
    // For DP bottom-up
    t = getTime();
    min = 10000;
    for (i = 6; i < 50; i++) {
        num = 0;
        Coins[2] = i;                     // For the coin set {1, 5, i, 50}
        // Determines the minimum number of coins to represent a integer 99
        NCoinDP_BU(99, 4, Coins);
        for (j = 1; j < 100; j++) {
            num += table[j];
        }
        if (num < min) {
            min = num;              // Record min
            x = i;                  // Record x
        }
    }
    t = getTime() - t;
    // Print the result
    printf("DP bottom-up: {1, 5, %d, 50} average is %.5f", x, (float)min / 99);
    printf(", CPU time: %g sec\n", t);

    // Replacing $10 and $50:
    printf("Replacing $10 and $50:\n  ");
    // For DP recursive
    t = getTime();
    min = 10000;
    for (i = 6; i < 99; i++) {
        Coins[2] = i;
        for (j = i + 1; j < 100; j++) {
            num = 0;
            Coins[3] = j;               // For the coin set {1, 5, i, j}
            // Determines the minimum number of coins to represent a integer k
            for (k = 1; k < 100; k++) {
                num += NCoinDP_R(k, 4, Coins);
            }
            if (num < min) {
                min = num;           // Record min
                x = i;               // Record x
                y = j;               // Record y
            }
        }
    }
    t = getTime() - t;
    // Print the result
    printf("DP recursive: {1, 5, %d, %d} ", x, y);
    printf("average is %.5f, CPU time: %g sec\n  ", (float)min / 99, t);
    // For DP top-down
    t = getTime();
    min = 10000;
    for (i = 6; i < 99; i++) {
        Coins[2] = i;
        for (j = i + 1; j < 100; j++) {
            // Initialize the g array
            for (k = 0; k < 4; k++) {
                for (l = 0; l < 100; l++){
                    g[k][l] = 0;
                    g[0][l] = l;          // g[0][D] = D
                }
            }
            num = 0;
            Coins[3] = j;                    // For the coin set {1, 5, i, j}
            // Determines the minimum number of coins to represent a integer k
            for (k = 1; k < 100; k++) {
                num += NCoinDP_TD(k, 4, Coins);
            }
            if (num < min) {
                min = num;           // Record min
                x = i;               // Record x
                y = j;               // Record y
            }
        }
    }
    t = getTime() - t;
    // Print the result
    printf("DP top-down : {1, 5, %d, %d} ", x, y);
    printf("average is %.5f, CPU time: %g sec\n  ", (float)min / 99, t);


    // For DP bottom-up
    t = getTime();
    min = 10000;
    for (i = 6; i < 99; i++) {
        Coins[2] = i;
        for (j = i + 1; j < 100; j++) {
            num = 0;
            Coins[3] = j;                 // For the coin set {1, 5, i, j}
            // Determines the minimum number of coins to represent a integer 99
            NCoinDP_BU(99, 4, Coins);
            for (k = 1; k < 100; k++) {
                num += table[k];
            }
            if (num < min) {
                min = num;             // Record min
                x = i;                 // Record x
                y = j;                 // Record y
            }
        }
    }
    t = getTime() - t;
    // Print the result
    printf("DP bottom-up: {1, 5, %d, %d} ", x, y);
    printf("average is %.5f, CPU time: %g sec\n  ", (float)min / 99, t);

    return 0;
}

double getTime(void)          // Get the time
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

int NCoinDP_TD(int D, int Ncoin, int Coins[])    // DP top-down approach
{
    int min;
    int x;

    min = D;                      // Initialize the min (only take $1)
    if (g[Ncoin - 1][D] > 0 || Ncoin == 1) {    // If the prior evaluation is done, return value
        return g[Ncoin - 1][D];
    }
    // Check all possible taking methods using recursion
    for (x = 0; x <= D / Coins[Ncoin - 1]; x++) {
        if (x + NCoinDP_TD(D - Coins[Ncoin - 1] * x, Ncoin - 1, Coins) < min) {
            min = x + NCoinDP_TD(D - Coins[Ncoin - 1] * x, Ncoin - 1, Coins);
        }
    }
    g[Ncoin - 1][D] = min;       // Record the min numbers in g array
    return min;
}

int NCoinDP_R(int D, int Ncoin, int Coins[])      // DP recursive approach
{
    int min;
    int x;

    min = D;             // Initialize the min (only take $1)
    if (Ncoin == 1) {
        return D;
    }
    // Check all possible taking methods using recursion
    for (x = 0; x <= D / Coins[Ncoin - 1]; x++) {
        if (x + NCoinDP_R(D - Coins[Ncoin - 1] * x, Ncoin - 1, Coins) < min) {
            min = x + NCoinDP_R(D - Coins[Ncoin - 1] * x, Ncoin - 1, Coins);
        }
    }
    return min;
}

void NCoinDP_BU(int D, int Ncoin, int Coins[]) // DP bottom-op approach
{
    int i, j;

    table[0] = 0;               // If D equals to 0
    for (i = 1; i <= D; i++) {  // Initialize the table (only take $1)
        table[i] = D;
    }
    // Compute minimum coins required for all values from 1 to D
    for (i = 1; i <= D; i++) {
        // Go through all coins smaller than Ncoin
        for (j = 0; j < Ncoin; j++) {
            if (Coins[j] <= i) {
                if (1 + table[i - Coins[j]] < table[i]) {
                    table[i] = 1 + table[i - Coins[j]];
                }
            }
        }
    }
}
