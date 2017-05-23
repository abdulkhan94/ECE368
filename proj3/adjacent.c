#include "adj.h"
#include <limits.h>
#include <math.h>

//This is basically used to create a new adjacency list node
struct AdjLN* new_AdjLN(int destination)
{
  //Variables
  struct AdjLN* nnode=(struct AdjLN*)malloc(sizeof(struct AdjLN));
  
  //Executions
  nnode->dest=destination;

  nnode->next=NULL;

  return nnode;
}

//This is basically used to create the graph. V is the number of vertices in the graph
struct Graph* create_Graph(int V)
{
  //Variables
  struct Graph* n_graph=(struct Graph*) malloc(sizeof(struct Graph));

  //Executions
  n_graph->ver=V;

  n_graph->array = (struct AdjL*) malloc(V * sizeof(struct AdjL));//Used to create an array with size V
  int ind=0;
  while(ind<V)//To initialize the list
    {
      n_graph->array[ind].head = NULL;
      ind=ind+1;
    }
  return n_graph;
}

//This function is used to make connections/edges between nodes. It accepts the two nodes the connection has to be made between. It makes a connection from source to destination and also from destination to source since the graph is undirected
void new_Edge(struct Graph* n_graph, int source, int destination, int weight)
{
  //Variables
  struct AdjLN* nnode=new_AdjLN(destination);

  //Executions
  nnode->next=n_graph->array[source].head;
  n_graph->array[source].head=nnode;
  nnode=new_AdjLN(source);
  nnode->next=n_graph->array[destination].head;
  n_graph->array[destination].head=nnode;
}



int main(int argc, char* argv[])
{
  char const* const fileName = argv[1]; /* should check that argc > 1 */
 
  int vertex, x_cord, y_cord;
 


  FILE* file = fopen(fileName, "r"); /* should check the result */
  int x;
  int y;


  rewind(file);
  fscanf(file, "%d %d", &x, &y);
  
  int i=0;
  struct AdjLN* positions  = (struct AdjLN*) malloc(x * sizeof(struct AdjLN));
  for(i =0; i<x; i++)
    {
        
      fscanf(file, "%d %d %d", &vertex, &x_cord, &y_cord);
        
      positions[i].v = vertex;
        
      positions[i].x_cord = x_cord;
        
      positions[i].y_cord = y_cord;
        
      printf("%d : %d %d \n",positions[i].v, positions[i].x_cord, positions[i].y_cord);
    }
  int j=0;
  int a = x;
  int b = y;
  int ind1 = 0;
  int ind2 = 0;
  int weight;
  struct Graph* graph = create_Graph(a);
  while(j<b)
    {      
      fscanf(file, "%d %d", &x, &y);
      while(i<a)  
	{
	  if(positions[i].v == y)        
	    {
       	      ind2 = i;
      	    }	  
	  if(positions[i].v == x)  
	    {
	      ind1 = i;        
	    }
       
	  i++;
  	}
        
      printf("%d coordinates(x,y) : (%d, %d)\n",positions[ind1].v,positions[ind1].x_cord, positions[ind1].y_cord);
      printf("%d coordinates(x,y) : (%d, %d)\n",positions[ind2].v,positions[ind2].x_cord, positions[ind2].y_cord); 
      weight = weight_calc(positions[ind1].x_cord,positions[ind1].y_cord,positions[ind2].x_cord,positions[ind2].y_cord); 
      printf("distance between %d and %d is %d \n", x, y, weight);
      new_Edge(graph, x, y, weight);
      j++;
    }
        
  print_Graph(graph);

  fclose(file);

  char const* const fp = argv[2];
  FILE* file2 = fopen(fp, "r");
  int queries;
  fscanf(file2,"%d", &queries);
  int src;
  int dest;
  i=0;
  
  for(i=0; i<queries; i++)
    {
        
      fscanf(file2,"%d %d", &src, &dest);
        
      dijkstra(graph, src, dest);
    }

  fclose(file2);
    
  return 0;

}
//CODE AFTER HERE

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
  struct MinHeapNode* t = *a;
  *a = *b;
  *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx)
{
  int smallest, left, right;
  smallest = idx;
  left = 2 * idx + 1;
  right = 2 * idx + 2;
 
  if (left < minHeap->size &&
      minHeap->array[left]->dist < minHeap->array[smallest]->dist )
    smallest = left;
 
  if (right < minHeap->size &&
      minHeap->array[right]->dist < minHeap->array[smallest]->dist )
    smallest = right;
 
  if (smallest != idx)
    {
      // The nodes to be swapped in min heap
      struct MinHeapNode *smallestNode = minHeap->array[smallest];
      struct MinHeapNode *idxNode = minHeap->array[idx];
 
      // Swap positions
      minHeap->pos[smallestNode->v] = idx;
      minHeap->pos[idxNode->v] = smallest;
 
      // Swap nodes
      swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
 
      minHeapify(minHeap, smallest);
    }
}
 
// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
  return minHeap->size == 0;
}
 
// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
  if (isEmpty(minHeap))
    return NULL;
 
  // Store the root node
  struct MinHeapNode* root = minHeap->array[0];
 
  // Replace root node with last node
  struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
  minHeap->array[0] = lastNode;
 
  // Update position of last node
  minHeap->pos[root->v] = minHeap->size-1;
  minHeap->pos[lastNode->v] = 0;
 
  // Reduce heap size and heapify root
  --minHeap->size;
  minHeapify(minHeap, 0);
 
  return root;
}
 
// Function to decreasy dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
  // Get the index of v in  heap array
  int i = minHeap->pos[v];
 
  // Get the node and update its dist value
  minHeap->array[i]->dist = dist;
  // Travel up while the complete tree is not hepified.
  while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
      // Swap this node with its parent
      minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
      minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
      swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
 
      // move to parent index
      i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
int isInMinHeap(struct MinHeap *minHeap, int v)
{
  if (minHeap->pos[v] < minHeap->size)
    return 1;
  return 0;
}
 
// A utility function used to print the solution
void printArr(int path[], int n,int array[], int src, int dest)
{
  int i;
  //printf("%d\n", dist);
  for(i = 0; i < n; ++i)
    {
      if(i == dest)
	{
        
	  printf("%d \n",path[i]);
        
	  printf("%d  ", array[i]); 
        
	}
    }

}

void dijkstra(struct Graph* graph, int src, int dest)
{
  int n = graph->ver;// Get the number of vertices in graph
  int path[n];      // path values used to pick minimum weight edge in cut
 
  struct MinHeap* minHeap = createMinHeap(n);
  int v;
  // Initializing min heap with all vertices
  for (v = 0; v < n; ++v)
    {
      path[v] = INT_MAX;
      minHeap->array[v] = newMinHeapNode(v, path[v]);
      minHeap->pos[v] = v;
    }
  minHeap->array[src] = newMinHeapNode(src, path[src]);
  minHeap->pos[src]   = src;
  path[src] = 0;
  decreaseKey(minHeap, src, path[src]);
 
  minHeap->size = n;
  int array[n];
  while (!isEmpty(minHeap))
    {
      // Extract the vertex with minimum distance value
      struct MinHeapNode* minHeapNode = extractMin(minHeap);
      int mv = minHeapNode->v; // Store the extracted vertex number
        
      printf("%d \n", minHeapNode->v);
        
      struct AdjLN* pCrawl = graph->AdjL[mv]->head;
        
      int i =0;
        
      //int array[n];
      while (pCrawl != NULL && i < n)
        {
	  int v = pCrawl->dest;
    
	  if (isInMinHeap(minHeap, v) && path[mv] != INT_MAX &&
	      pCrawl->weight + path[mv] < path[v])
            {
	      path[v] = path[mv] + pCrawl->weight;
        
        
	      array[i] = mv;
        
        
	      i++;
        
        
	      printf("%d \n", mv);
	      decreaseKey(minHeap, v, path[v]);
            }
	  pCrawl = pCrawl->next;
        }
    }
  printArr(path,n,array, src,dest);
}

int weight_calc(int a, int b, int c, int d)
{
  int weight =  (((d-b)*(d-b))+((c-a)*(c-a)));
  int weight_sqrt = sqrt(weight);
  return weight_sqrt;
}
