// EE3980 HW02 Random Data Searches
// 107061240,
// 2021/03/20

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
#include <time.h>
int j;                                 // Variable for random odd-even search

double get_time(void);                            // Do local time in seconds
void read_input(char **data, int n);              // Read all inputs
int Search(char *word, char **list, int n);       // Linear Search
int Search2(char *word, char **list, int n);      // Search 2 Algorithm
int OEsearch(char *word, char **list, int n);     // odd-even Search
int ROEsearch(char *word, char **list, int n);    // randomized Odd-even Search

int main(void)
{
    int i;                           // Loop index
    int R = 500;                     // Number of repetitions
    double t;                        // Counting time
    char **data;                     // Input data
    int N;                           // Input size
    int k[500];                      // Random number array
    int res;                         // Searching result

    scanf("%d", &N);                           // Numbers of the string
    srand(time(NULL));
    // Give space to the data and list
    data = (char **)malloc(N * sizeof(char *));
    // Establish an array of random number for measuring average cases
    for (i = 0; i < R; i++){
        k[i] = rand() % N;
    }
    read_input(data, N);                  // Read the input into data array
    printf("n: %d\n", N);                 // Print the size of the data set

    // Perform linear search for average case
    t = get_time();
    for (i = 0; i < R; i++){
        res = Search(data[k[i]], data, N);
    }
    t = (get_time() - t) / R;
    printf("Linear search average CPU time: %g\n", t);
    // Perform search 2 for average case
    t = get_time();
    for (i = 0; i < R; i++){
        res = Search2(data[k[i]], data, N);
    }
    t = (get_time() - t) / R;
    printf("Search 2 Algorithm average CPU time: %g\n", t);
    // Perform odd-even search for average case
    t = get_time();
    for (i = 0; i < R; i++){
        res = OEsearch(data[k[i]], data, N);
    }
    t = (get_time() - t) / R;
    printf("Odd-even search average CPU time: %g\n", t);
    // Perform random odd-even search for average case
    t = get_time();
    for (i = 0; i < R; i++){
        j = rand() % 2;
        res = ROEsearch(data[k[i]], data, N);
    }
    t = (get_time() - t) / R;
    printf("Random odd-even search average CPU time: %g\n", t);


    R = 5000;                      // Repeat for 5000 times
    // Perform the linear search for the worst case
    t = get_time();
    for (i = 0; i < R; i++){
        res = Search(data[N - 1], data, N);
    }
    t = (get_time() - t) / R;
    printf("Linear search worst-case CPU time: %g\n", t);
    // Perform the search 2 for the worst case
    t = get_time();
    for (i = 0; i < R; i++){
        res = Search(data[N - 1], data, N);
    }
    t = (get_time() - t) / R;
    printf("Search 2 Algorithm worst-case CPU time: %g\n", t);
    // Perform the odd-even search for the worst case
    t = get_time();
    for (i = 0; i < R; i++){
        res = OEsearch(data[N - 2], data, N);
    }
    t = (get_time() - t) / R;
    printf("Odd-even search worst-case CPU time: %g\n", t);
    // Perform the random odd-even search for the worst case
    t = get_time();
    for (i = 0; i < R; i++){
        j = rand() % 2;
        if (j == 1){
            res = ROEsearch(data[N - 2], data, N);
        }
        else
            res = ROEsearch(data[N - 1], data, N);
    }
    t = (get_time() - t) / R;
    printf("Random odd-even search worst-case CPU time: %g\n", t);

    return 0;
}

double get_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}
void read_input(char **data, int n)
{
    int i;
    int len;                   // String size
    char str[30];              // String to store the scan value
    // Scan the string and copy to the array data
    for (i = 0; i < n; i++) {
        scanf(" %s", str);
        len  = strlen(str) + 1;
        data[i] = (char *)malloc(len * sizeof(char));
        data[i] = str;
    }
}
int Search(char *word, char **list, int n)
{
    int i;

    for (i = 0; i < n; i++){
        if (strcmp(list[i], word) == 0){        // Compare all possible entries
            return i;
        }
    }
    return -1;                                  // Unsuccessful search
}

int Search2(char *word, char **list, int n)
{
    int i;

    for (i = 0; i < n; i = i + 2){          // Compare all possible entries
        if (strcmp(list[i], word) == 0){
            return i;
        }
        if (strcmp(list[i + 1], word) == 0){
            return i + 1;
        }
    }
    return -1;                                  // Unsuccessful search
}
int OEsearch(char *word, char **list, int n)
{
    int i;
    for (i = 1; i < n; i = i + 2){          // Compare odd entries
        if (strcmp(list[i], word) == 0){
            return i;
        }
    }
    for (i = 0; i < n; i = i + 2){          // Compare even entries
        if (strcmp(list[i], word) == 0){
            return i;
        }
    }
    return -1;                                  // Unsuccessful search
}
int ROEsearch(char *word, char **list, int n)
{
    int i;

    if (j == 1){
        for (i = 1; i < n; i = i + 2){          // Compare odd entries
            if (strcmp(list[i], word) == 0){
                return i;
            }
        }
        for (i = 0; i < n; i = i + 2){          // Compare even entries
            if (strcmp(list[i], word) == 0){
                return i;
            }
        }
    }
    else {
        for (i = 0; i < n; i = i + 2){          // Compare even entries
            if (strcmp(list[i], word) == 0){
            return i;
            }
        }
        for (i = 1; i < n; i = i + 2){          // Compare odd entries
            if (strcmp(list[i], word) == 0){
                return i;
            }
        }
    }
}


