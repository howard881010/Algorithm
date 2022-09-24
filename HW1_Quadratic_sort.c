// EE3980 HW01 Quadratic Sorts
// 107061240, ½²©¾¯E
// 2021/03/13

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

int N;                                        // Input size


void read_input(char **data);                 // Read all inputs
void print_array(char **list);                // Print the content of array A
void copy_array(char **data, char **list);    // Copy data to list
double get_time(void);                        // Do local time in seconds
void selection_sort(char **list, int n);      // Do selection sort
void insertion_sort(char **list, int n);      // Do insertion sort
void bubble_sort(char **list, int n);         // DO bubble sort
void oddeven_sort(char **list, int n);        // Do shaker sort
void swap(char **list, int a, int b);

void swap(char **list, int a, int b) {        //Swap the given value
    char temp[30];
    strcpy(temp, list[a]);
    strcpy(list[a], list[b]);
    strcpy(list[b], temp);
}


void read_input(char **data)
{
    int i;
    char str[30];              // String to store the scan value
    // Scan the string and copy to the array data
    for (i = 0; i < N; i++) {
        scanf(" %s", str);
        strcpy(data[i], str);
    }
}

void print_array(char **list)
{
    int i;
    // Print the sorted array one by one
    for (i = 1; i <= N; i++) {
        printf ("%d %s\n", i, list[i - 1]);
    }
}

void copy_array(char **data, char **list)
{
    int i;
    //One by one copy the data's string to list
    for ( i = 0; i < N; i++) {
        strcpy(list[i], data[i]);
    }
}

double get_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void selection_sort(char **list, int n)
{
    int i, j, min_idx;
    // One by one move boundary of unsorted array
    for (i = 0; i < N - 1; i++)  {
        min_idx = i;
        //Find the minimum element in array list
        for (j = i + 1; j < N; j++) {
            if (strcmp(list[min_idx], list[j]) > 0) {
                min_idx = j;
            }
        }
        // Swap the found minimum element with the first element
        if (min_idx != i) {
            swap(list, min_idx, i);
        }
    }
}
void insertion_sort(char **list, int n)
{
    int i, j;                       // Loop index
    char str[30];                   // String for comparing


    for (i = 1; i < n; i++) {       // Assume list[0:i - 1] already sorted
        strcpy(str, list[i]);       // Move list[i] to its proper place
        j = i - 1;                  // Initialize j to be i - 1
        // Find j such that list[i] < list[j]
        while ((j >= 0) && (strcmp(str, list[j]) < 0)) {
            strcpy(list[j + 1], list[j]);  // Move list[j] up by one position
            j--;
       }
      strcpy(list[j + 1], str);    // Move list[j] to list[j + 1]
    }
}

void bubble_sort(char **list, int n)
{
    int i, j;                             // Loop index

    for (i = 0; i < n - 1; i++) {         // Find the smallest item for list[i]
        for (j = n - 1; j > i; j--) {
            if (strcmp(list[j], list[j - 1]) < 0) {
                swap(list, j, j - 1);     // Swap list[j] and list[j -1]
           }
       }
    }
}

void oddeven_sort(char **list, int n)
{
    bool done = false;                               // Array is unsorted
    int i;                                           // Loop index

    while (!done) {
        done = true;
        // Do bubble sort on odd indexed element
        for (i = 1; i < n - 1; i = i + 2) {
            if (strcmp(list[i], list[i + 1]) > 0) {
                swap(list, i, i + 1);
                done = false;
            }
        }
        // Do bubble sort on odd indexed element
        for (i = 0; i < n - 1; i = i + 2) {
            if (strcmp(list[i], list[i + 1]) > 0) {
                swap(list, i, i + 1);
                done = false;
            }
        }
    }
}


int main(void)
{
    int i;                                     // Loop index
    int R = 500;                               // Number of repetitions
    double t;                                  // Counting time
    char **data;                               // Input data
    char **list;                               // Array to be sorted

    scanf("%d", &N);                           // Numbers of the string
    // Give space to the data and list
    data = (char **)malloc(N * sizeof(char *));
    list = (char **)malloc(N * sizeof(char *));
    for (i = 0; i < N; i++){
        data[i] = (char *)malloc(30 * sizeof(char));
        list[i] = (char *)malloc(30 * sizeof(char));
    }

    read_input(data);                                         // Read input
    // Do selection sort for R times
    t = get_time();
    for (i = 0; i < R; i++) {
        copy_array(data, list);
        selection_sort(list, N);
    }
    t = (get_time() - t) / R;
    printf("Selection sort: N = 10 CPU = %f seconds\n", t);
    // Doing insertion sort for R times
    t = get_time();
    for (i = 0; i < R; i++) {
        copy_array(data, list);
        insertion_sort(list, N);
    }
    t = (get_time() - t) / R;
    printf("Insertion sort: N = 10 CPU = %f seconds\n", t);
    // Doing bubble sort for R times
    t = get_time();
    for (i = 0; i < R; i++) {
        copy_array(data, list);
        bubble_sort(list, N);
    }
    t = (get_time() - t) / R;
    printf("Bubble sort: N = 10 CPU = %f seconds\n", t);
    // Doing oddeven sort for R times
    t = get_time();
    for (i = 0; i < R; i++) {
        copy_array(data, list);
        oddeven_sort(list, N);
    }
    t = (get_time() - t) / R;
    printf("OddEven sort: N = 10 CPU = %f seconds\n", t);

    print_array(list);               // Print the final result

    return 0;
}
