// EE3980 HW05 Better Sorts
// 107061240,
// 2021/04/10

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

int N;                            // Input size
char **B;                         // String array to store the result of Merge


void read_input(char **data);                 // Read all inputs
void print_array(char **list);                // Print the content of array list
void copy_array(char **data, char **list);    // Copy data to list
double get_time(void);                        // Do local time in seconds
void HeapSort(char **list, int n);            // Do HeapSort
// To enforce max heap property for n-element heap list with root i
void Heapify(char **list, int i, int n);
void MergeSort(char **list, int low, int high);   // Do MergeSort
// Merge sorted list[low : mid] and list[mid + 1: high] to nondecreasing order
void Merge(char **list, int low, int mid, int high);
void QuickSort(char **list, int low, int high);   // Do QuickSort
// Partition list into list[low : mid − 1] ≤ list[mid]
// and list[mid + 1 : high] ≥ list[mid].
int Partition(char **list, int low, int high);


int main(void)
{
    int i;                                     // Loop index
    int R = 500;                               // Number of repetitions
    double t;                                  // Counting time
    char **data;                               // Input data
    char **list;                               // Array to be sorted

    scanf("%d", &N);                           // Numbers of the string
    // Give space to data, list, B
    data = (char **)malloc(N * sizeof(char *));
    // N + 1 for QuickSort, which needs list[N] to be infinite
    list = (char **)malloc((N + 1) * sizeof(char *));
    B = (char **)malloc(N * sizeof(char *));
    read_input(data);                          // Read input
    printf("N = %d\n", N);
    // Allocate space to list, B arrays
    for (i = 0; i < N; i++) {
        list[i] = (char *)malloc((strlen(data[i]) + 1) * sizeof(char));
        B[i] = (char *)malloc((strlen(data[i]) + 1) * sizeof(char));
    }
    // Execute HeapSort 500 times & print the result
    t = get_time();
    for (i = 0; i < R; i++) {
        copy_array(data, list);
        HeapSort(list, N);
    }
    t = (get_time() - t) / R;
    printf("HeapSort CPU time: %g s\n", t);
    // Execute MergeSort 500 times & print the result
    t = get_time();
    for (i = 0; i < R; i++) {
        copy_array(data, list);
        MergeSort(list, 0, N - 1);
    }
    t = (get_time() - t) / R;
    printf("MergeSort CPU time: %g s\n", t);

    list[N] = "zzzzzzzzzzzzz";           // Initialize list[N] to be infinite
    // Execute QuickSort 500 times & print the result
    t = get_time();
    for (i = 0; i < R; i++) {
        copy_array(data, list);
        QuickSort(list, 0, N - 1);
    }
    t = (get_time() - t) / R;
    printf("QuickSort CPU time: %g s\n", t);

    print_array(list);                // Print the final result

    return 0;
}

double get_time(void)        // Do local time in seconds
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void print_array(char **list)   // Print the content of array list
{
    int i;

    // Print the sorted array one by one
    for (i = 1; i <= N; i++) {
        printf ("%d %s\n", i, list[i - 1]);
    }
}

void read_input(char **data)    // Read all input data
{
    int i;
    char str[30];              // String to store the scan value
    int len;

    // Scan the string and copy to the array data
    for (i = 0; i < N; i++) {
        scanf(" %s", str);
        len = strlen(str) + 1;
        data[i] = (char *)malloc(len * sizeof(char));
        strcpy(data[i], str);
    }
}

void copy_array(char **data, char **list)   // Copy data into list
{
    int i;

    //One by one copy the data's string to list
    for (i = 0; i < N; i++) {
        list[i] = data[i];
    }
}

void HeapSort(char **list, int n)       // Execute HeapSort
{
    int i;                            // Loop index
    char *temp;                       // Variable for swap

    // Initialize list[0 : n - 1] to be a max heap
    for (i = (n - 1) / 2; i >= 0; i--) {
        Heapify(list,i ,n);
    }
    for (i = n - 1; i > 0; i--) {     // Repeat n − 1 times
        temp = list[0];               // Move maximum to the end.
        list[0] = list[i];
        list[i] = temp;
        Heapify(list, 0, i);          // Then make list[0 : i − 2] a max heap
    }
}

void Heapify(char **list, int i, int n)
{
    int j;
    char *item;
    bool done;

    j = 2 * i + 1;               // list[j] is the lchild.
    item = list[i];
    done = false;
    while (j < n && (!done)) {   // list[j + 1] is the rchild.
        if (j < n - 1 && (strcmp(list[j], list[j + 1]) < 0)) {
            j = j + 1;           // list[j] is the larger child.
        }
        if (strcmp(item, list[j]) > 0) {   // If larger than children, done
            done = true;
        }
        else {     // Otherwise, continue
            list[(j - 1) / 2] = list[j];
            j = 2 * j + 1;
        }
    }
    list[(j - 1) / 2] = item;
}

void MergeSort(char **list, int low, int high)  //Execute MergeSort
{
    int mid;

    if (low < high) {
        mid = (high + low) / 2;
        MergeSort(list, low, mid);              // Left region
        MergeSort(list, mid + 1, high);         // Right region
        Merge(list, low, mid, high);
    }
}

void Merge(char **list, int low, int mid, int high)  // Execute Merge function
{
    int h, i, j;
    int k;

    // Initialize looping indices.
    h = low;
    i = low;
    j = mid + 1;
    while ((h <= mid) && (j <= high)) {       // Store smaller one to B[i]
        if (strcmp(list[h], list[j]) <= 0) {  // list[h] is smaller
            B[i] = list[h];
            h++;
        }
        else {                                // list[j] is smaller
            B[i] = list[j];
            j++;
        }
        i++;
    }
    if (h > mid) {         // list[j : high] remaining.
        for (k = j; k <= high; k++) {
            B[j] = list[k];
            i++;
        }
    }
    else {                 // list[h : mid] remaining.
        for (k = h; k <= mid; k++) {
            B[i] = list[k];
            i++;
        }
    }
    for (k = low; k <= high; k++) {   // Copy B to list
        list[k] = B[k];
    }
}

void QuickSort(char **list, int low, int high)   // Execute QuickSort
{
    int mid;

    if (low < high) {
        mid = Partition(list, low, high + 1);
        QuickSort(list, low, mid - 1);         // Left region
        QuickSort(list, mid + 1, high);        // Right region
    }
}

int Partition(char **list, int low, int high)   // Execute Partition function
{
    char *v;
    char *temp;
    int i, j;

    // Initialize
    v = list[low];
    i = low;
    j = high;
    do {   // Check for all elements
        do {    // Find i such that list[i] ≥ v
            i++;
        } while (strcmp(list[i], v) < 0);
        do {    //  Find j such that list[j] ≤ v
            j--;
        } while (strcmp(list[j], v) > 0);
        if (i < j) {    // Exchange list[i] and list[j]
            temp = list[i];
            list[i] = list[j];
            list[j] = temp;
        }
    } while (i < j);
    // Move v to the right position
    list[low] = list[j];
    list[j] = v;

    return j;
}
