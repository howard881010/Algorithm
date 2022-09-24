// EE3980 Project 1. Coin Picking Game
// 107061240,
// 2021/5/29

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define m 1000000


int CoinLeft(int A[], int low, int high); // Selecting left coin
int CoinRandom(int A[], int low, int high); // Random selection
int CoinGreedy(int A[], int low, int high); // Greedy approach
int CoinDP(int A[], int low, int high); // Dynamic programming
// m coin-picking games played using FX1 and FX2 functions
double compete(int (*FX1)(int *, int, int), int (*FX2)(int *, int, int));
int min(int a, int b);    // Find the min
int max(int a, int b);    // Find the max


int main(void)
{
    double win1;          // The win rate of the Player 1

    srand(time(NULL));
    win1 = compete(CoinDP, CoinDP);     // Start competing
    printf("Player 1: %.2f%\n", win1 * 100);
    printf("Player 2: %.2f%\n", 100 - win1 * 100);

    return 0;
}

double compete(int (*FX1)(int *, int, int), int (*FX2)(int *, int, int))
{
    int win1, win2;
    int sum1, sum2;
    int low, high;
    int g, r, i;
    int k1, k2;
    int A[10];

    win1 = 0;          // Initialize wins to 0
    win2 = 0;
    for (g = 0; g < m; g++) {
        for (i = 0; i < 10; i++) {    // Initialize coins
            A[i] = rand() % 4 + 1;
            if (A[i] == 2) {
                A[i] = 5;
            } else if (A[i] == 3) {
                A[i] = 10;
            } else if (A[i] == 4) {
                A[i] = 50;
            }
            //printf("%d\n", A[i]);
        }
        sum1 = 0;      // Initialize sum and ranges
        sum2 = 0;
        low = 0;
        high = 9;
        for (r = 0; r < 5; r++) {    // 5 picks for each player
            //printf("low = %d high = %d\n", low, high);
            k1 = FX1(A, low, high);  // Player 1 plays
            //printf("k1 = %d\n", k1);
            if (k1 == low) {
                sum1 += A[low];
                low++;
            } else {
                sum1 += A[high];
                high--;
            }
            //printf("low = %d high = %d\n", low, high);
            k2 = FX2(A, low, high);  // Player 2 plays
            //printf("k2 = %d\n", k2);
            if (k2 == low) {
                sum2 += A[low];
                low++;
            } else {
                sum2 += A[high];
                high--;
            }
        }
        if (sum1 > sum2) {
            win1++;
        } else if (sum1 < sum2) {
            win2++;
        }
        //printf("%%%%%%%%%%\n");
    }

    return (float) win1 / (win1 + win2);  // Winning ration of player 1
}


int CoinLeft(int A[], int low, int high)  // Selecting left coin
{
    return low;  // Choose the coin at the left end
}

int CoinRandom(int A[], int low, int high) // Random selection
{
    int i;

    i = rand() % 2;     // Randomize
    if (i == 0) {
        return low;     // Choose the coin at the left end
    }

    return high;        // Choose the coin at the right end
}

int CoinGreedy(int A[], int low, int high) // Greedy approach
{
    if (A[low] < A[high]) {
        return high;   // Choose the coin at the right end
    }

    return low;        // Choose the coin at the left end
}

int CoinDP(int A[], int low, int high)
{
    int n = high - low + 1;
    int table[n][n];           // table to record the optimal value
    int num;                   // Number of element to choose
    int i, j;                  // Loop index
    int x, y, z;               // Variable to represent the table's value
    int temp;                  // Record the index

    if (low == high) {         // Only one element
        return low;
    }
    for (i = 0; i < n; i++) {     // Initialize the table
        table[i][i] = A[i + low];
    }
    // Determine the number of element to choose
    for (num = 2; num <= n; num++) {
        for (i = 0; i < n - num + 1; i++) {  // To fill up the table
            j = i + num - 1;
            if(j == i + 1) {     // Only two element
                // Table's values equals larger one
                table[i][j]= max(A[i + low],A[j + low]);
                if (A[i + low] > A[j + low]) {   // Return the larger one
                    temp = i + low;
                }
                else {
                    temp = j + low;
                }
            }
            else {
                // Represent the equation
                x = table[i + 2][j];
                y = table[i + 1][j - 1];
                z = table[i][j - 2];
                table[i][j] = max(A[i + low] + min(x, y), A[j + low] + min(y, z));
                // Return the larger one
                if (A[i + low] + min(x, y) > A[j + low] + min(y, z)) {
                    temp = i + low;
                } else {
                    temp = j + low;
                }
            }
        }
    }
    return temp;
}

int min(int a, int b)    // Find the smaller value
{
    if (a > b) {
        return b;
    }

    return a;
}

int max(int a, int b)   // Find the larger value
{
    if (a > b) {
        return a;
    }

    return b;
}


