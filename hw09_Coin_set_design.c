// EE3980 HW09 Coin Set Design
// 107061240,
// 2021/5/15

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

// Determines the minimum number of coins to represent a integer D
int NCoinGreedy(int D, int NCoin, int Coins[]);



int main()
{
    int i, j, k;
    int Coins[4] = {1, 5, 10, 50};
    int num;
    int min;
    int x, y;

    // For coin set {1, 5, 10, 50}
    num = 0;
    // Determines the minimum number of coins to represent a integer i
    for (i = 1; i < 100; i++) {
        num += NCoinGreedy(i, 4, Coins);
    }
    printf("For coin set {1, 5, 10, 50}", x);
    printf(" the average is %.5f\n", (float)num / 99);


    // For coin set  {1, 5, 10, dd}
    min = 10000;
    for (i = 11; i < 100; i++) {  // Find all possible number of i
        Coins[3] = i;             // Let Conis[3] be i
        num = 0;
        // Determines the minimum number of coins to represent a integer j
        for (j = 1; j < 100; j++) {
            num += NCoinGreedy(j, 4, Coins);
        }
        if (num < min) {
            min = num;
            x = i;                // Record the number of i
        }
    }
    printf("Coin set {1, 5, 10, %d}", x);
    printf(" has the minimum average of %.5f\n", (float)min / 99);

    // For coin set  {1, 5, dd, 50}
    Coins[3] = 50;
    min = 10000;
    for (i = 6; i < 50; i++) {    // Find all possible number of i
        Coins[2] = i;             // Let Conis[3] be i
        num = 0;
        // Determines the minimum number of coins to represent a integer j
        for (j = 1; j < 100; j++) {
            num += NCoinGreedy(j, 4, Coins);
        }
        if (num < min) {
            min = num;
            x = i;                 // Record the number of i
        }
    }
    printf("Coin set {1, 5, %d, 50}", x);
    printf(" has the minimum average of %.5f\n", (float)min / 99);


    // For coin set {1, 5, dd, dd}
    min = 10000;
    for (i = 6; i < 99; i++) {             // Find all possible number of i
        Coins[2] = i;                      // Let Coins[2] be i
        for (j = i + 1; j < 100; j++) {    // Find all possible number of j
            Coins[3] = j;                  // Let Coins[3] be j
            num = 0;
            // Determines the minimum number of coins to represent a integer k
            for (k = 1; k < 100; k++) {
                num += NCoinGreedy(k, 4, Coins);
            }
            if (num < min) {
                min = num;
                x = i;              // Record the number of i
                y = j;              // Record the number of j
            }
        }
    }
    printf("Coin set {1, 5, %d, %d}", x, y);
    printf(" has the minimum average of %.5f\n", (float)min / 99);

    return 0;
}

int NCoinGreedy(int D, int NCoin, int Coins[])
    // Determines the minimum number of coins to represent a integer D
{
    int i, k, num;

    num = 0;    // Initialize the number of coins
    //printf("D = %d", D);
    for (i = NCoin - 1; i >= 0; i--) {   // Take the largest coin first

        while (D >= Coins[i]) {          // Take the coin until D < Coin
            D = D - Coins[i];
            num++;
        }
    }
    //printf(" %d\n", num);

    return num;
}
