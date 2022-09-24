// EE3980 HW11 Travelling Salesperson Problem
// 107061240,
// 2021/6/12

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define inf 10000000

int N;
char **city;
char **result;
int size = 25;
int *path;
int c = 0;

struct Node
{
    int cost;           // Store the lower bound
    int vertex;        // Store the current city number
    int level;           // Store the total number of city so far
    int **r_adj;      // Store the reduced matrix
};

struct Heap {
    int front, rear, size, cap;
    struct Node **node;
};

int TSP(int **adj);
void rowReduction(int **adj, int *row);
void columnReduction(int **adj, int *col);
int calculateCost(int **adj);
struct Node* newNode(int **adj, int level, int i, int j);
void enHeap(struct Heap *a, struct Node *node, int n);
struct Node* top(struct Heap *a);
void HeapRmMax(struct Heap *a, int n);
void Heapify(struct Heap *a, int i, int n);

int main(void)
{
    int **adj;
    int i, j;

    scanf("%d\n", &N);
    adj = (int **)malloc(N * sizeof(int *));
    city = (char **)malloc(N * sizeof(char *));
    result = (char **)malloc(N * sizeof(char *));
    path = (int *)malloc(N * sizeof(int));
    for (i = 0; i < N; i++) {
        adj[i] = (int *)malloc(N * sizeof(int));
        city[i] = (char *)malloc(25 * sizeof(char));
        result[i] = (char *)malloc(N * sizeof(char));
        fgets(city[i], size, stdin);
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < 25; j++) {
            if (city[i][j] != '\n') {
                 result[i][j] = city[i][j];
            }
            else {
                result[i][j] = '\0';
                j = 40;
            }
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            scanf("%d", &adj[i][j]);
            if (adj[i][j] == 0) {
                adj[i][j] = inf;
            }
        }
    }
    //printf("1111");
    int ans = TSP(adj);

    printf("Minimum distance route:\n");
    for (i = 0; i < N - 1; i++) {
        printf("  %d -> %d\n", path[i] + 1, path[i + 1] + 1);
    }
    printf("  %d -> %d\n",path[N - 1] + 1, path[0] + 1);
    printf("Total travelling distance: %d\n", ans);

    return 0;
}

int TSP(int **adj)
{
    struct Node *root;
    struct Node *min;
    struct Node *child;
    int i, j;
    struct Heap* a;

    a = (struct Heap*)malloc(sizeof(struct Heap));

    //a->front = 0;
    a->size = 0;
    //a->rear = - 1;
    a->node = (struct Node**)malloc(inf * sizeof(struct Node*));
    min = (struct Node*)malloc(sizeof(struct Node));
    printf("%d", a->size);
    root = newNode(adj, 0, -1, 0);
    // get the lower bound of the path starting at node 0
    root->cost = calculateCost(root->r_adj);

    enHeap(a, root, a->size);// Add root to the list of live nodes
    // Finds a live node with the least cost, adds its children to the list of
    // live nodes, and finally deletes it from the list


    while(a->size != 0) {
        printf("%d %d size = %d\n", a->front,  a->rear, a->size);
        //min = top(a);      // Find a live node with the least estimated cost
        HeapRmMax(a, a->size);
        printf("\n\n\n");
        i = min->vertex;         // `i` stores the current city number
        if(min->level == N - 1) {
            return min->cost;
        }
        a->size = 0;
        a->front = 0;
        a->rear = - 1;
        for (j = 0; j < N; j++) {
            if(min->r_adj[i][j] != inf) {
                child = newNode(min->r_adj, min->level + 1, i, j);
                child->cost = min->cost + min->r_adj[i][j] + calculateCost(child->r_adj);
                enHeap(a, child, a->size);
            }
        }
    }
    return 0;
}

int calculateCost(int **adj)
{
    int *row;
    int *col;
    int i;
    int cost;

    row= (int *)malloc(N * sizeof(int));
    col = (int *)malloc(N * sizeof(int));
    for (i = 0; i < N; i++) {
        row[i] = inf;
        col[i] = inf;
    }
    rowReduction(adj, row);
    columnReduction(adj, col);
    cost = 0;
    for (i = 0; i < N; i++) {
        if (row[i] != inf) {
            cost += row[i];
        }
        if (col[i] != inf) {
            cost += col[i];
        }
    }

    return cost;
}

void rowReduction(int **adj, int *row)
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (adj[i][j] < row[i]) {
                row[i] = adj[i][j];
            }
        }
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (adj[i][j] != inf && row[i] != inf) {
                adj[i][j] -= row[i];
            }
        }
    }
}

void columnReduction(int **adj, int *col)
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (adj[i][j] < col[j]) {
                col[j] = adj[i][j];  // col[j] contains minimum in col j
            }
        }
    }
    // reduce the minimum value from each element in each column
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (adj[i][j] != inf && col[j] != inf) {
                adj[i][j] -= col[j];
            }
        }
    }
}

struct Node* newNode(int **adj, int level, int row, int col)
{
    struct Node *node;
    int i, j;

    node = (struct Node*)malloc(sizeof(struct Node));
    node->r_adj = (int **)malloc(N * sizeof(int *));
    for (i = 0; i < N; i++) {
        node->r_adj[i] = (int *)malloc(N * sizeof(int));
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            node->r_adj[i][j] = adj[i][j];
        }
    }
    for (i = 0; level != 0 && i < N; i++) {
        node->r_adj[row][i] = inf;
        node->r_adj[i][col] = inf;
    }
    node->r_adj[col][0] = inf;  // set r_adj[j][0] to infinity
    node->level = level;        // set number of cities visited so far
    node->vertex = col;        // assign current city number

    return node;
};

void enHeap(struct Heap *a, struct Node *node, int n)
{
    int i;

    a->size = n + 1;
    i = a->size;
    a->node[n + 1] = node;
    while ((i > 1) && a->node[i / 2]->cost > node->cost) {
        a->node[i] = a->node[i / 2];
    }
    a->node[i] = node;
}

void HeapRmMax(struct Heap *a, int n)
{
    a->node[1] = a->node[n];
    Heapify(a, 1, n - 1);
}

void Heapify(struct Heap *a, int i, int n)
{
    int j;
    struct Node *item;
    bool done;

    j = 2 * i;                   // list[j] is the lchild.
    item = a->node[i];
    done = false;
    while (j <= n && (!done)) {   // list[j + 1] is the rchild
        if (j < n &&  a->node[j]->cost >= a->node[j + 1]->cost) {
            j = j + 1;           // list[j] is the smaller child.
        }
        if (item->cost <= a->node[j]->cost) {   // If smaller than children, done
            done = true;
        }
        else {     // Otherwise, continue
            a->node[j / 2] = a->node[j];
            j = 2 * j;
        }
    }
    a->node[j / 2] = item;
}
