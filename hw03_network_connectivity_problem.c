/* EE3980 HW03 Network Connectivity Problem
 * 107061240
 * 2021/03/27
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#define Re 10

int V, E, NS;					  // Nodes, edges, and sets
int *G1, *G2;					  // Store the right and left node of edges
int *R, *P;					    // Root, and count-root array

void readGraph(void);		 // Read the graph data
double getTime(void);		 // Get local time in second
void Connect1(void);		    // Use SetFind and SetUnion function
void Connect2(void);		    // Use WeightedUnion and SetUnion function
void Connect3(void);	 	    // Use WeightedUnion and CollapsingFind function
void Setunion(int i, int j);      // Connect two set
int SetFind(int i);			          // Find the root
void WeightedUnion(int i, int j);	// Another way to connect two sets
int collapsingfind(int i);		    // Another way to find the root

int main(void)
{
    double t0, t1, t2, t3;		  // Record the CPU time
    int i;                      // Loop index
    int NS1, NS2, NS3;          // Disjoint sets


    readGraph();                  // Read the input graph
    // Use connect1 to connect the data
    t0 = getTime();
    for (i = 0; i < 10; i++) {
        Connect1();
    }
    t1 = getTime();
    NS1 = NS;
    // Use connect2 to connect the data
    for (i = 0; i < Re; i++) {
        Connect2();
    }
    t2 = getTime();
    NS2 = NS;
    // Use connect3 to connect the data
    for (i = 0; i < Re; i++) {
        Connect3();
    }
    t3 = getTime();
    NS3 = NS;
    // Print the final result
    printf("|V| = %d, |E| = %d\n", V, E);
    printf("Connect1 CPU time = %g, Disjoint sets: %d\n", (t1 - t0)/ Re, NS1);
    printf("Connect2 CPU time = %g, Disjoint sets: %d\n", (t2 - t1)/ Re, NS2);
    printf("Connect3 CPU time = %g, Disjoint sets: %d\n", (t3 - t2)/ Re, NS3);

    return 0;
}


void readGraph(void)			            // Read the graph data
{
    int i;                                // Loop index

    scanf("%d %d\n", &V, &E);	            // Scan the vertexes and edges
    // Allocate the space for data
    G1 = (int *)malloc(E * sizeof( int ));
    G2 = (int *)malloc(E * sizeof( int ));
    R = (int *)malloc(V * sizeof( int ));
    P = (int *)malloc(V * sizeof( int ));

    for (i = 0; i < E; i++) {			 // Read all inputs into G1 and G2 array
        scanf("%d %d", &G1[i], &G2[i]);
        G1[i]--;
        G2[i]--;
    }
}

double getTime(void)          // Get the time
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

int SetFind(int i)				      //Find the root of integer i
{
    while (P[i] >= 0) {         // P[i] is not -1
        i = P[i];                     // Go to the parent of P[i]
    }
    return i;
}

void Setunion(int i, int j)		  // Unite two disjoint sets
{
    P[i] = j;                   // Let j be the parent of P[i]
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

void Connect1(void)             // Use the Connect1 to connect the data
{
    int i;                      // Loop index
    int x, y;                   // Variable to store the root

    for (i = 0; i < V; i++) {		// Reset the R and P array
        R[i] = i;
        P[i] = -1;
    }
    NS = V;							          // Number of disjoint sets
    for (i = 0; i < E; i++) { 		// Connected vertices
        x = SetFind(G1[i]);
        y = SetFind(G2[i]);
        if ( x != y ) {           // Unite two sets
            NS = NS - 1;          // Number of disjoint sets decreases by 1
            Setunion(x, y);
        }
    }
    for (i = 0; i < V; i++) {     // Record root to R table
        R[i] = SetFind(i);
    }

}

void Connect2(void)               // Use the Connect2 to connect the data
{
    int i;                        // Loop index
    int x, y;                     // Variable to store the root

    for (i = 0; i < V; i++) {  		// Reset R and P array
        R[i] = i;
        P[i] = -1;
    }
    NS = V;							        // Number of disjoint sets
    for (i = 0; i < E; i++) 		// Connected vertices
    {
        x = SetFind(G1[i]);
        y = SetFind(G2[i]);
        if ( x != y ) {         // Unite two sets
            NS = NS-1;				  // Number of disjoint sets decrease by 1
            WeightedUnion(x, y);
        }
    }
    for (i = 0; i < V; i++) {   // Record root to R table
        R[i] = SetFind(i);
    }

}

void Connect3(void)             // Use the Connect2 to connect the data
{
    int i;                      // Loop index
    int x, y;                   // Variable to store the root

    for (i = 0; i < V; i++)  		// Reset R and P array
    {
        R[i] = i;
        P[i] = -1;
    }
    NS = V;						             // Number of disjoint sets
    for (i = 0; i < E; i++) {	     // Connected vertices
        x = CollapsingFind(G1[i]);
        y = CollapsingFind(G2[i]);
        if ( x != y ) {            // Unite two sets
            NS = NS-1;			       // Number of disjoint sets decrease by 1
            WeightedUnion(x, y);
        }
    }
    for (i = 0; i < V; i++) {      // Record root to R table
        R[i] = SetFind(i);
    }
}

