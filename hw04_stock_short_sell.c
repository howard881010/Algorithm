// EE3980 HW04 Stock Short Selling Revisited
// 107061240,
// 2021/4/17


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#define Re 1000

typedef struct sSTKprice {				// a structure to store date and price
    int year, month, day;				// and change of everyday
    double price, change;
} STKprice;

double getTime(void);		    // get local time in second
STKprice *read(int total);		// read the graph data
//Brute-force Approach function
double MaxSubArrayBF(STKprice A[], int n, int *low, int *high);
//Divide and Conquer function
double MaxSubArray(STKprice A[], int begin, int end, int *low, int *high);
//Cross Boundary
double MaxSubArrayXB(STKprice A[], int begin, int mid, int end,
                      int *low, int *high);


int main(void)
{
    int total, i, low, high;
    double max, t0, t1;
    STKprice *s;

    scanf("%d\n", &total);		// Scan the number of total day of the data
    s = read(total);
    printf("N = %d\n", total);

    low = 0;
    high = total - 1;
    t0 = getTime();				// Record the time of the beginning
    max = MaxSubArrayBF(s, total, &low, &high);	// Call the BF function
    printf("low :%d, high: %d\n", low, high);
    // Because the change is i-(i-1), the low need to -1 to get the sell day
    low--;
    t1 = getTime();				// Record the end time
    // Print the outcome
    printf("Brute-force approach: CPU time %g s\n", t1 - t0);
    printf("Sell: %d/%d/%d",s[low].year, s[low].month, s[low].day);
    printf(" at %g\n", s[low].price);
    printf("Buy: %d/%d/%d",s[high].year, s[high].month, s[high].day);
    printf(" at %g\n", s[high].price);
    printf("Earning: %g per share\n", max);

    // Initialize
    low = 0;
    high = total - 1;
    t0 = getTime();				// Record the time of the beginning
    for (i = 0; i < Re; i++) {		// Repeat function 1000 times
        max = MaxSubArray(s, 0, total - 1, &low, &high);
    }
    low--;
    t1 = getTime();				// Record the end time.
    // Print the outcome
    printf("Divide and Conquer: CPU time %g s\n", (t1 - t0) / Re);
    printf("Sell: %d/%d/%d",s[low].year, s[low].month, s[low].day);
    printf(" at %g\n", s[low].price);
    printf("Buy: %d/%d/%d",s[high].year, s[high].month, s[high].day);
    printf(" at %g\n", s[high].price);
    printf("Earning: %g per share\n", max);
    return 0;
}

STKprice *read(int total)      // Read the graph data
{
    int i;
    STKprice *s;

    // Allocate spaces for the structure
    s = (STKprice *)malloc( total * sizeof(STKprice) );
    for (i = 0; i < total; i++) {	// Read data into s dynamic array
        scanf("%d", &s[i].year);
        scanf("%d", &s[i].month);
        scanf("%d", &s[i].day);
        scanf("%lf", &s[i].price);
    }

    s[0].change = 0;
    for (i = 1; i < total; i++) {         // Count change of everyday price
        s[i].change = -(s[i].price - s[i - 1].price);
    }

    return s;
}

double getTime(void)          // Get the time
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

double MaxSubArrayBF(STKprice A[], int n, int *low, int *high)
{
    double max, sum;
    int i, j, k;

    //  Initialize
    max = 0;
    *low = 0;
    *high = n - 1;
    for (j = 0; j < n; j++) {		// Try all possible ranges: A[j : k]
        for (k = j; k < n; k++) {
            sum = 0;
            for (i = j; i <= k; i++) {   // Summation for A[j : k ]
                sum = sum + A[i].change;
            }
            if (sum > max) {		// Record the minimum value and range
                max = sum;
                *low = j;
                *high = k;
            }
        }
    }

    return max;
}

double MaxSubArray(STKprice A[], int begin, int end, int *low, int *high)
{
    int mid;
    int llow, lhigh;
    int rlow, rhigh;
    int xlow, xhigh;
    double lsum, rsum, xsum;

    // Initialize
    mid = (begin + end) / 2;
    llow = begin, lhigh = mid;
    rlow = mid + 1;
    rhigh = end;
    xlow = mid;
    xhigh = mid + 1;
    if (begin == end) {							// Termination condition
        *low = begin;
        *high = end;
        return A[begin].change;
    }
    lsum = MaxSubArray(A, begin, mid, &llow, &lhigh);         // Left region
    rsum = MaxSubArray(A, mid + 1, end, &rlow, &rhigh);       // Right region
    xsum = MaxSubArrayXB(A, begin, mid, end, &xlow, &xhigh);  // Cross boundary
    if (lsum >= rsum && lsum >= xsum) {			// rsum is the largest
        *low = llow;
        *high = lhigh;
        return lsum;
    }
    else if (rsum >= lsum && rsum >= xsum) {		// lsum is the largest
        *low = rlow;
        *high = rhigh;
        return rsum;
    }
    *low = xlow;
    *high = xhigh;

    return xsum;		// Cross-boundary is the largest
}

double MaxSubArrayXB(STKprice A[], int begin, int mid,
                     int end, int *low, int *high)
{
    int i;
    double lsum, sum, rsum;

    // Initialize for lower half
    lsum = 0;
    *low = mid;
    sum = 0;

    for (i = mid; i >= begin; i--) {// Find low to maximize sum of A[low : mid]
        sum = sum + A[i].change;    // Continue to add
        if (sum > lsum) {		    // Record if larger
            lsum = sum;
            *low = i;
        }
    }
    // Initialize for higher half
    rsum = 0;
    *high = mid + 1;
    sum = 0;
    for (i = mid + 1; i <= end; i++) {
    // Find end to maximize sum of A[mid + 1 : high]
        sum = sum + A[i].change;				// Continue to add
        if (sum > rsum) {						// Record if larger
            rsum = sum;
            *high = i;
        }
    }

    return (lsum + rsum);						// Overall sum
}


