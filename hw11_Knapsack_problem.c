// EE3980 HW11 0/1 Knapsack Problem
// 107061240,
// 2021/6/05

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

int fp, fw;              // final profit, final weight
int *idx;                // index of item
double *list;       // Store the profits per weight
int m;                   // maximum weight
int num;               // numbers of items
int *cx;                 // current solution
int *x;                   // solution
int *w;                  // weight array
int *p;                   // profits array

struct item {
    char name[3];         // item's name
    int w;                        // item's weight
    int p;                         // item's profits
};

void Heapify(double *list, int i, int n);     // Maintain the min heap property
void HeapSort(double *list, int n);          // Do HeapSort
double getTime(void);		                       // Get local time in second
void BKnap(int k, int cp, int cw);     // find solution of 0/1 knapsack problem
// estimate maximum profit for k + 1 to n objects
int Bound(int cp, int cw, int k);

int main(void)
{
    int i;                              // loop index
    struct item *item;           // item's pointer
    double t;                     // record the time

    scanf("%d %d", &num, &m);    // read in numbers of items and maximum weight
    // allocate the space
    item = (struct item*)malloc((num + 1) * sizeof(struct item));
    list = (double *)malloc((num + 1) * sizeof(double));
    idx = (int *)malloc((num + 1) * sizeof(int));
    cx = (int *)malloc((num + 1) * sizeof(int));
    x = (int *)malloc((num + 1) * sizeof(int));
    w = (int *)malloc((num + 1) * sizeof(int));
    p = (int *)malloc((num + 1) * sizeof(int));
    for (i = 1; i <= num; i++) {     // read in the information of items
        scanf("%s %d %d", &item[i].name, &item[i].w, &item[i].p);
        list[i] = (float)item[i].p / item[i].w;      // let list = profit / weight
        idx[i] = i;
    }
    t = getTime();
    fp = 0;              // initialize
    fw = 0;
    HeapSort(list, num);
    printf("Pick items:\n");
    for (i = 1; i <= num; i++) {
        w[i] = item[idx[i]].w;
        p[i] = item[idx[i]].p;
    }
    BKnap(1, 0, 0);
    t = getTime() - t;
    for (i = 1; i <= num; i++) {
        if (x[i] == 1) {
            printf("  %s %d %d\n", item[idx[i]].name, w[i], p[i]);
        }
    }
    printf("N = %d, M = %d\n", num, m);
    printf("Weight = %d, Profits = %d\n", fw, fp);
    printf("CPU time: %g sec\n", t);



    return 0;
}

double getTime(void)          // Get the time
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void HeapSort(double *list, int n)       // Execute HeapSort
{
    int i;                            // Loop idx
    double temp;                      // Variable for swap
    int t;

    // Initialize list[0 : n - 1] to be a max heap
    for (i = n / 2; i > 0; i--) {
        Heapify(list,i ,n);
    }
    for (i = n; i > 1; i--) {     // Repeat n ? 1 times
        temp = list[i];           // Move maximum to the end
        list[i] = list[1];
        list[1] = temp;
        t = idx[i];
        idx[i] = idx[1];
        idx[1] = t;
        Heapify(list, 1, i - 1);     // Then make list[1 : i ? 1] a max heap
    }
}

void Heapify(double *list, int i, int n)      // Maintain the min heap property
{
    int j;
    double item;
    bool done;
    int c;

    j = 2 * i;                   // list[j] is the lchild.
    item = list[i];
    c = idx[i];
    done = false;
    while (j <= n && (!done)) {   // list[j + 1] is the rchild
        if (j < n &&  list[j] < list[j + 1]) {
            j = j + 1;           // list[j] is the smaller child.
        }
        if (item > list[j]) {   // If smaller than children, done
            done = true;
        }
        else {     // Otherwise, continue
            list[j / 2] = list[j];
            idx[j / 2] = idx[j];
            j = 2 * j;
        }
    }
    list[j / 2] = item;
    idx[j / 2] = c;
}

void BKnap(int k, int cp, int cw)
{
    int i;

    if (cw + w[k] <= m) {
        cx[k] = 1;
        if (k < num) {
            BKnap(k + 1, cp + p[k], cw + w[k]);
        }
        else if ((cp + p[k] > fp) && (k == num)) {
            fp = cp + p[k];
            fw = cw + w[k];
            for (i = 1; i <= num; i++) {
                x[i] = cx[i];
            }
        }
    }
    if (Bound(cp, cw, k) >= fp) {
        cx[k] = 0;
        if (k < num) {
            BKnap(k + 1, cp, cw);
        }
        else if ((cp > fp) && (k == num)) {
            fp = cp;
            fw = cw;
            for (i = 1; i <= num; i++) {
                x[i] = cx[i];
            }
        }
    }
}

int Bound(int cp, int cw, int k)
{
    int mp, mw;
    int i;

    mp = cp;
    mw = cw;
    for (i = k + 1; i <= num; i++) {
        mw += w[i];
        if (mw < m) {
            mp += p[i];
        }
        else {
            return mp + (1 - (mw - m) / w[i]) * p[i];
        }
    }
    return mp;
}

