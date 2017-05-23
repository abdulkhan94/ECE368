#include <stdio.h>
#include <stdlib.h>

//Structure to represent the graph
struct Graph
{
  int ver;
  struct AdjL* array;
};

//Structure to represent adjacency list node
struct AdjLN
{
  int dest;
  struct AdjLN* next;
  int v;
  int x_cord;
  int y_cord;
  int weight;
};

//Structure to represent adjacency list
struct AdjL 
{
  struct AdjLN *head;
};


void new_Edge(struct Graph* n_graph, int source, int destination, int weight);

void print_Graph(struct Graph* n_graph);


int weight_calc(int a, int b, int c, int d);

// Structure to represent a min heap node
struct MinHeapNode
{
    int  v;
    int dist;
};
 
// Structure to represent a min heap
struct MinHeap
{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};
 
// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}
 
// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);

void minHeapify(struct MinHeap* minHeap, int idx);

int isEmpty(struct MinHeap* minHeap);

struct MinHeapNode* extractMin(struct MinHeap* minHeap);

void decreaseKey(struct MinHeap* minHeap, int v, int dist);

int isInMinHeap(struct MinHeap *minHeap, int v);

void printArr(int path[], int n,int array[], int src, int dest);

void dijkstra(struct Graph* graph, int src, int dest);
