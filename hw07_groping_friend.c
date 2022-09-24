// EE3980 HW07 Grouping Friends
// 107061240,
// 2021/5/1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

int *f;               // Finished time
int ftime;
int *visited;         // Recored whether we visit the element
int *B;               // Integer array to store the result of Merge
int *A;               // Determine the order of DFS
int *P;               // Predecessor array
int *R;               // Root array
int *root;            // Record the final root
int *num;

struct Person {       // Data of person
    char name[14];    // Name of person
    int num;          // Convert the name to the index for convenient execution
    struct Person* receiver;    // Record receiver
};

struct subgroup {          // Data of subgroups
    int index;             // Record the index of subgroup
    struct subgroup* next;
};

struct Message {           // Data of messages
    char sender[14];       // Name of sender
    char receiver[14];     // name of receiver
    int s, r;    // Convert the name to the index for convenient execution
};

struct Graph {             // Graph
    int N;                 // Number of person
    struct Person** adjList;   // Link list of person's data
};

struct group {              // Group
    int N;                  // Number of subgroup
    struct subgroup** subg;  // Link list of subgroup's data
};

double getTime(void);		    // Get local time in second
struct Person* read(int N);		    // Read the people data
struct Message* read1(int M, struct Person *p, int N);     // Read the message
struct Graph* CreateGraph(int N);                // Create a graph
struct group* CreateGroup(int N);                // Create a group
void addEdge(struct Graph* g, int s, int r);    // Add the message to the Graph
void addPerson(struct group* gp, int r, int i);  // Add a person to the Group
void printgroup(struct group *gp, struct Person *p);  // Print the final result
void DFS(struct Graph *g);                    // Execute the Depth first search
void DFS_d(int v, struct Graph *g);    // More sophisticated Depth first search
void Heapify(int *list, int i, int n);          // Maintain the min heap property
void HeapSort(int *list, int n);            // Do HeapSort

int CollapsingFind(int i);             // Do CollapsingFind to find the root



int main(void)
{
    int i, j, n;                  // Loop index
    int N;                        // Number of people
    int M;                        // Number of messages
    int *list;                    // Sorted finished time
    int n_g;                      // Number of Subgroup
    double t;                     // Record time
    struct Person *p;
    struct Message *m;
    struct Message *mt;
    struct Graph *g;
    struct Graph *gt;
    struct group *gp;

    scanf("%d %d", &N, &M);      // Scan the data
    // Allocate the space for the array
    A = (int *)malloc(N * sizeof(int));
    B = (int *)malloc(N * sizeof(int));
    P = (int *)malloc(N * sizeof(int));
    f = (int *)malloc(N * sizeof(int));
    R = (int *)malloc(N * sizeof(int));
    visited = (int *)malloc(N * sizeof(int));
    list = (int *)malloc((N + 1) * sizeof(int));
    num = (int *)malloc((N + 1) * sizeof(int));
    // Read in the people's name and messages
    p = read(N);
    m = read1(M, p, N);

    // Start to recored execution time
    t = getTime();
    // Create the graph and the transpose graph
    g = CreateGraph(N);
    gt = CreateGraph(N);
    // Add messages to the graph and the transpose graph
    for (i = 0; i < M; i++){
        addEdge(g, m[i].s, m[i].r);
        addEdge(gt, m[i].r, m[i].s);
    }
    // Initialize the order of Depth first search of the graph and roots
    for (i = 0; i < N; i++) {
        A[i] = i;
        num[i + 1] = i;
        R[i] = -1;
    }
    DFS(g);                   // Implement the Depth first search of the graph
    for (i = 1; i <= N ; i++) {   // Copy the finished time to list[i]
        list[i] = f[i - 1];
        //printf("i = %d %d\n", A[i - 1], list[i]);
    }

    //printf("\n");
    HeapSort(list, N);
    for (i = 1; i <= N; i++) {
        A[i - 1] = num[i];
        //printf("i = %d %d\n", A[i - 1], list[i]);
    }
    DFS(gt);      // Implement the Depth first search of the transpose graph
    // Find the root
    for (i = 0; i < N; i++) {
        R[i] = CollapsingFind(i);
        //printf("%d\n", R[i]);
    }
    n_g = 0;
    for (i = 0; i < N; i++) {
        if (R[i] == i) {
            n_g++;
        }
    }

    gp = CreateGroup(n_g);
    root = (int *)malloc(n_g * sizeof(int));

    n = 0;
    for (i = 0; i < N; i++) {
        if (R[i] == i) {
            root[n] = R[i];
            n++;
        }
    }
    for (j = 0; j < n_g; j++) {
        for (i = 0; i < N; i++) {
            if (root[j] == R[i] && R[i] != i) {
                addPerson(gp, j, i);
            }
        }
    }
    t = getTime() - t;

    printf("N = %d M = %d Subgroup = %d CPU time = %g\n", N, M, n_g, t);
    printf("Number of subgroups: %d\n", n_g);
    printgroup(gp, p);

    return 0;
}


double getTime(void)          // Get the time
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

struct Person* read(int N)
{
    int i;
    struct Person *p;

    p = (struct Person*)malloc(N * sizeof(struct Person));
    for (i = 0; i < N; i++) {
        scanf("%s", p[i].name);
        p[i].num = i;
        p[i].receiver = NULL;
    }

    return p;
}

struct Message* read1(int M, struct Person* p, int N)
{
    int i;
    int j;
    struct Message *m;

    m = (struct Message *)malloc(M * sizeof(struct Message));
    for (i = 0; i < M; i++) {
        scanf("%s -> %s", m[i].sender, m[i].receiver);
        for (j = 0; j < N; j++) {
            if (strcmp(p[j].name, m[i].sender) == 0) {
                m[i].s = j;
            }
            else if (strcmp(p[j].name, m[i].receiver) == 0) {
                m[i].r = j;
            }
        }
        //printf("%s -> %s, %d -> %d\n",m[i].sender, m[i].receiver, m[i].s, m[i].r);
    }

    return m;
}
struct Graph* CreateGraph(int N)
{
    int i;
    struct Graph *g;

    g = (struct Graph *)malloc(sizeof(struct Graph));
    g->N = N;
    g->adjList = (struct Person**)malloc(N * sizeof(struct Person*));
    for(i = 0; i < N; i++) {
        g->adjList[i] = NULL;
    }

    return g;
}

struct group* CreateGroup(int N)
{
    int i;
    struct group *gp;

    gp = (struct group *)malloc(sizeof(struct group));
    gp->N = N;
    gp->subg = (struct subgroup**)malloc(N * sizeof(struct subgroup*));
    for (i = 0; i < N; i++) {
        gp->subg[i] = NULL;
    }
    return gp;

}

void addEdge(struct Graph* g, int s, int r)
{
    struct Person* newp;

    newp = (struct Person*)malloc(sizeof(struct Person));
    newp->num = r;
    newp->receiver = g->adjList[s];
    g->adjList[s] = newp;
}

void addPerson(struct group* gp, int r, int i)
{
    struct subgroup* newsubg;

    newsubg = (struct subgroup*)malloc(sizeof(struct subgroup));
    newsubg->index = i;
    newsubg->next = gp->subg[r];
    gp->subg[r] = newsubg;
}


void printgroup(struct group* gp, struct Person* p) {
  int v;

  for (v = 0; v < gp->N; v++) {
    struct subgroup* temp = gp->subg[v];
    printf("  Subgroup %d: %s ", v + 1, p[root[v]].name);
    while (temp) {
      printf("%s ", p[temp->index].name);
      temp = temp->next;
    }
    printf("\n");
    }
}

void DFS(struct Graph* g)
{
    int i;

    for (i = 0; i < g->N; i++) {
        visited[i] = 0;
        f[i] = 0;
        P[i] = -1;
    }
    ftime = 0;
    for (i = 0; i < g->N; i++) {
        if (visited[A[i]] == 0) {
            //printf("%d", A[i]);
            DFS_d(A[i], g);
        }
    }
}

void DFS_d(int v, struct Graph* g)
{
    int i;
    struct Person* temp;

    visited[v] = 1;
    ftime = ftime + 1;
    temp = g->adjList[v];
    for (; temp != NULL; temp = temp->receiver) {
        if(visited[temp->num] == 0) {
            P[temp->num] = v;
            DFS_d(temp->num, g);
        }
    }
    visited[v] = 2;
    ftime = ftime + 1;
    f[v] = ftime;

}

void HeapSort(int *list, int n)       // Execute HeapSort
{
    int i;                            // Loop index
    int temp;                       // Variable for swap
    int t;

    // Initialize list[0 : n - 1] to be a max heap
    for (i = n / 2; i > 0; i--) {
        Heapify(list,i ,n);
    }
    for (i = n; i > 1; i--) {     // Repeat n ? 1 times
        temp = list[i];               // Move maximum to the end.
        list[i] = list[1];
        list[1] = temp;
        t = num[i];
        num[i] = num[1];
        num[1] = t;
        Heapify(list, 1, i - 1);          // Then make list[1 : i ? 1] a max heap
    }
}

void Heapify(int *list, int i, int n)      // Maintain the min heap property
{
    int j;
    double item;
    bool done;
    int c;

    j = 2 * i;                   // list[j] is the lchild.
    item = list[i];
    c = num[i];
    done = false;
    while (j <= n && (!done)) {   // list[j + 1] is the rchild
        if (j < n &&  list[j] >= list[j + 1]) {
            j = j + 1;           // list[j] is the smaller child.
        }
        if (item <= list[j]) {   // If smaller than children, done
            done = true;
        }
        else {     // Otherwise, continue
            list[j / 2] = list[j];
            num[j / 2] = num[j];
            j = 2 * j;
        }
    }
    list[j / 2] = item;
    num[j / 2] = c;
}

int CollapsingFind(int i)
// Find the root of i, and collapsing the elements on the path
{
    int r, s;

    r = i;                     // Initialized r to i
    while (P[r] >= 0) {        // Find the root
        r = P[r];
    }
    while (i != r) {           // Collapse the elements of the path
        s = P[i];
        P[i] = r;
        i = s;
    }
    return r;
}
