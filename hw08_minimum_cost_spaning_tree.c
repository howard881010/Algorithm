// EE3980 HW08 Minimum-Cost Spanning Tree
// 107061240,
// 2021/5/8

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

int *P;              // Parent array
double *list;        // Store the sorted edge's weight
int *num;            // Store the index of sorted edge

struct Edge {          // A structure to represent a weighted edge in graph
    int a, b;          // Edge's direction from a to b
    double weight;
};

struct Node {          // A structure to represent Node in graph
    int dest;
    struct Node* next;
};

struct Graph {         // A structure graph
    int V, E;          // Number of vertexes, edges
    struct Node** array;
};

struct Graph* createGraph(int V, int E);      // Construct an empty graph
void addEdge(struct Graph* g, int s, int r);  // Add the message to the Graph
double getTime(void);                         // Get local time in second
double Kruskal(struct Graph* g, struct Edge* e);  // Do Kruskal's algorithm
void printGraph(struct Graph* graph);
void Heapify(int i, int n);          // Maintain the min heap property
int CollapsingFind(int i);           // Do CollapsingFind to find the root
void WeightedUnion(int i, int j);	 // Connect two sets
double HeapRmMin(int n);             // Remove the minimum in the heap

int main(void)
{
    int V, E;                 // Number of vertexes, edges
    struct Graph* g;          // Graph pointer
    int i;                    // Loop index
    struct Edge* e;           // Edge pointer
    double cost;              // Store the minimum cost
    double t;              // Store the execution time

    scanf("%d %d", &V, &E);    // Read in the number of vertexes, edges
    // Allocate the space for the array
    e = (struct Edge*)malloc((E + 1) * sizeof(struct Edge));
    P = (int *)malloc((V + 1) * sizeof(int));
    num = (int *)malloc((E + 1) * sizeof(int));
    list = (double *)malloc((E + 1) * sizeof(double));

    g = createGraph(V, E);   // Create the graph
    for (i = 1; i <= E; i++) {
        // Read in information of edges and add them to the graph
        scanf("%d %d %lf", &e[i].a, &e[i].b, &e[i].weight);
        addEdge(g, e[i].a, e[i].b);
        list[i] = e[i].weight;       // Unsorted edges' weight
        num[i] = i;                  // Unsorted edges' indices
    }
    //printGraph(g);
    // DO Kruskal's algorithm to get the minimum cost
    printf("Minimum-cost spanning tree:\n");
    t = get_time();
    cost = Kruskal(g, e);
    t = get_time() - t;
    printf("|V| = %d |E| = %d\n", V, E);
    printf("%.2f\n", cost);
    printf("CPU time: %g\n", t);

    return 0;
}

double getTime(void)          // Get the time
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

struct Graph* createGraph(int V, int E)       // Create an empty graph
{
    int i;
    struct Graph* graph;

    graph = (struct Graph*)(malloc(sizeof(struct Graph)));
    graph->V = V;      // Initialize the number of vertexes
    graph->E = E;      // Initialize the number of edges
    graph->array = (struct Node**)malloc((V + 1) * sizeof(struct Node*));
    for(i = 0; i <= V; i++) {    // Initialize the link list in Group
        graph->array[i] = NULL;
    }

    return graph;
};

void addEdge(struct Graph* g, int s, int r) // Add a message into the Graph
{
    struct Node* newnode;

    newnode = (struct Node*)malloc(sizeof(struct Node));
    newnode->dest = r;          // Initialize the edge's destination
    newnode->next = g->array[s];  // Add a edge into the link list
    g->array[s] = newnode;
}

double Kruskal(struct Graph* g, struct Edge *e)  // DO Kruskal's algorithm
{
    int i;            // Loop index
    double mincost;   // Store the minimum cost
    double cost;      // Store the minimum weight in heap array
    int j, k;         // Store the root
    int u, v;         // Direction of the edges

    // Initialize list[1 : n] to be a min heap
    for (i = g->E / 2; i > 0; i--) {
        Heapify(i, g->E);
    }
    for (i = 1; i <= g->V; i++) {
        P[i] = -1;          // Enable cycle checking
    }
    i = 0;
    mincost = 0;
    while ((i < g->V - 1) && (g->E >= 0)) {
        //printf("num[1] = %d\n", num[1]);
        u = e[num[1]].a;       // Initialize the u and v
        v = e[num[1]].b;
        // Delete a minimum cost edge (u, v) from the heap
        cost = HeapRmMin(g->E);
        g->E--;   // Reduce the heap's size
        j = CollapsingFind(u);    // Using parent array
        k = CollapsingFind(v);

        //printf("u = %d v = %d\n", u, v);
        if (j != k) {
            i++;
            printf("%d: <%d %d> %.2f\n", i, u, v, cost);
            mincost = mincost + cost;
            WeightedUnion(j, k);   // Modify parent array
        }
    }

    return mincost;       // Return the minimum cost
}

void Heapify(int i, int n)      // Maintain the min heap property
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

double HeapRmMin(int n)   // Remove and return the minimum of the heap array
{
    double x;

    x = list[1];          // Remove x
    list[1] = list[n];
    num[1] = num[n];
    Heapify(1, n - 1);    // Maintain the min heap property

    return x;             // Return x
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

void WeightedUnion(int i, int j)
    // Unite two disjoint sets, using the weighting rule
{
    int temp;

    temp = P[i] + P[j];         // Note that temp < 0
    if (P[i] > P[j]) {          // i has fewer elements
        P[i] = j;               // j become the root of i
        P[j] = temp;
    }
    else {                      // j has fewer elements
        P[j] = i;
        P[i] = temp;
    }
}
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 1; v <= graph->V; ++v)
    {
        struct Node* temp = graph->array[v];
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (temp)
        {
            printf("%d ->", temp->dest);
            temp = temp->next;
        }
        printf("\n");
    }
}


