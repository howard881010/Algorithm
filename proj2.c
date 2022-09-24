// EE3980 Project 2. Encoding Text Files
// 107061240,
// 2021/6/14

int totalnum, encodenum;
int cnt = 131;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Node {
    char data[4];
    int freq;
    struct Node *left, *right;
};

struct MinHeap {
    int size;
    int cap;
    struct Node **arr;
};


struct Node* newNode(char *data, int freq);
// Maintain the min heap property
void Heapify(int *list, char  **letter, int i, int n);
void HeapSort(int *list, char **letter, int n);            // Do HeapSort
void readText(int *freq, char **letter);


void minHeapify(struct MinHeap* minH, int idx);
struct Node* least(struct MinHeap* minH);
void enMinHeap(struct MinHeap* minH, struct Node* Node);
struct MinHeap* BuildMinHeap(char **data, int *freq, int size);
struct Node* Tree(char **data, int *freq, int size);
void HuffmanCodes(char **data, int *freq, int size);
void printCodes(struct Node* root, int *arr, int top);

int main(void)
{
    int *freq;
    char **letter;
    int *freq1;
    char **letter1;
    int start = 1;
    int i;



    totalnum = encodenum = 0;
    freq = (int *)malloc(10000 * sizeof(int));
    letter = (char **)malloc(10000 * sizeof(char *));
    freq1 = (int *)malloc(10000 * sizeof(int));
    letter1 = (char **)malloc(10000 * sizeof(char *));
    for (i = 0; i < 10000; i++) {
        letter[i] = (char *)malloc(4 * sizeof(char));
        letter1[i] = (char *)malloc(4 * sizeof(char));
        letter[i][0]= '\0';
        letter1[i][0]= '\0';
        freq[i] = 0;
    }
    readText(freq, letter);
    printf("cnt = %d", cnt);
    for (i = 0; i < 10000; i++) {
        if (freq[i] != 0) {
            freq1[start] = freq[i];
            letter1[start] = letter[i];
            start++;
        }
    }
    printf("\\n");
    printf("%d\n", start);
    HeapSort(freq1, letter1, start - 1);

    for (i = 1; i < start; i++) {
        if (letter1[i][0] == ' ') {
            printf("the freq of ' ' is %d\n", freq1[i]);
        }
        else if (letter1[i][0] =='\n') {
            printf("the freq of '/n' is %d\n", freq1[i]);
        }
        else
            printf("the freq of %s is %d\n", letter1[i], freq1[i]);
    }

    HuffmanCodes(letter1, freq1, start - 1);
    printf("%d\n", encodenum / 8 + 1);
    printf("%d", encodenum);


    return 0;
}

void readText(int *freq, char **letter)
{
    char ch;
    char chin[4];
    int i;

    totalnum = 0;
    while (scanf("%c", &ch) != EOF) {
        totalnum++;
        if ((ch & 0xf0) == 0xe0) {
            for (i = 131; i <= cnt; i++) {
                chin[0] = ch;
                scanf("%c", &chin[1]);
                scanf("%c", &chin[2]);
                chin[3] = '\0';
                //if (strcmp(chin, letter[i]) != 0) {
                    //letter[i] = chin;
                    //cnt++;
                    //i = cnt + 1;
                    //freq[i]++;
                //}
                //else if (strcmp(chin, letter[i]) == 0) {
                    //freq[i]++;
                //}
            }
        }
        else {
            letter[(int)ch][0] = (int)ch;
            letter[(int)ch][1] = '\0';
            freq[(int)ch]++;
        }
    }
}

void HeapSort(int *list, char **letter, int n)       // Execute HeapSort
{
    int i;                            // Loop index
    int temp;                       // Variable for swap
    char *t;

    // Initialize list[0 : n - 1] to be a max heap
    for (i = n / 2; i > 0; i--) {
        Heapify(list, letter, i ,n);
    }
    for (i = n; i > 1; i--) {     // Repeat n - 1 times
        temp = list[i];               // Move maximum to the end.
        list[i] = list[1];
        list[1] = temp;
        t = letter[i];
        letter[i] = letter[1];
        letter[1] = t;
        Heapify(list, letter,  1, i - 1);          // Then make list[1 : i ? 1] a max heap
    }
}

void Heapify(int *list, char **letter, int i, int n)      // Maintain the min heap property
{
    int j;
    int item;
    bool done;
    char *c;

    j = 2 * i;                   // list[j] is the lchild.
    item = list[i];
    c = letter[i];
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
            letter[j / 2] = letter[j];
            j = 2 * j;
        }
    }
    list[j / 2] = item;
    letter[j / 2] = c;
}

struct Node* newNode(char *data, int freq)
{
    struct Node* temp;
    int i;

    temp = (struct Node*)malloc(sizeof(struct Node));
    temp->left = temp->right = NULL;
    for (i = 0; i < 4; i++) {
        temp->data[i] = data[i];
    }
    temp->freq = freq;

    return temp;
}

void minHeapify(struct MinHeap* minH, int idx)
{
    int j = 2 * idx;
    struct Node* item = minH->arr[idx];
    bool done = false;

    while (j <= minH->size && !done) {
        if (j < minH->size && minH->arr[j]->freq >= minH->arr[j + 1]->freq) {
            j ++;
        }
        if (item->freq < minH->arr[j]->freq) {
            done = true;
        }
        else {
            minH->arr[j / 2] = minH->arr[j];
            j = j * 2;
        }
    }
    minH->arr[j / 2] = item;
}

struct Node* least(struct MinHeap* minH)
{
    struct Node* x;

    x = minH->arr[1];
    minH->arr[1] = minH->arr[minH->size];
    minH->size--;
    minHeapify(minH, 1);

    return x;
}

void enMinHeap(struct MinHeap* minH, struct Node* Node)
{
    int i;

    minH->size++;
    i = minH->size;
    //printf("size = %d\n", minH->size);
    minH->arr[minH->size] = Node;
    while (i > 1 && minH->arr[i/2]->freq >= Node->freq) {
        minH->arr[i] = minH->arr[i / 2];
        i = i / 2;
    }
    minH->arr[i] = Node;
}

struct MinHeap* BuildMinHeap(char **data, int *freq, int size)
{
    struct MinHeap* minH;
    int i;

    minH = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minH->size = 0;           // Current size is 0
    minH->cap = size;
    minH->arr = (struct Node**)malloc((size + 1) * sizeof(struct Node*));

    for (i = 1; i <= size; i++) {
        minH->arr[i] = newNode(data[i], freq[i]);
        //printf("%c %d\n", data[i], freq[i]);
    }
    //printf("%d", size);
    minH->size = size;
    for (i = minH->size / 2; i > 0; i--) {
        minHeapify(minH, i);
    }

    return minH;
}

struct Node* Tree(char **data, int *freq, int size)
{
    struct Node *left, *right, *top;
    struct MinHeap* minH;

    minH = BuildMinHeap(data, freq, size);

    while (minH->size != 1) {
        left = least(minH);
        right = least(minH);
        //printf("right = %d left = %d\n", right->freq, left->freq);

        top = newNode(left->data, left->freq + right->freq);
        top->left = left;
        top->right = right;

        enMinHeap(minH, top);
    }

    return least(minH);
}

void HuffmanCodes(char **data, int *freq, int size)
{
    struct Node* root;
    root = Tree(data, freq, size);
    int arr[100];
    printCodes(root, arr, 0);
}

void printCodes(struct Node* root, int *arr, int top)
{
    int i;

    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    if (!(root->left) && !(root->right)) {
        printf("%s:", root->data);
        for (i = 0; i < top; i++) {
            printf("%d ", arr[i]);

        }
        encodenum += root->freq * top;
        printf("freq = %d", root->freq);
        printf("\n");
    }
}


