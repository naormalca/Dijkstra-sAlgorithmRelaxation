#include <stdio.h>
#include <stdlib.h>
/*
	Naor Malca -  204608657
	Oryan Morad - 313138653
	*/
//heap sort
#define HEAP_PARENT(i) (i/2)
#define HEAP_LEFT(i) (2*i)
#define HEAP_RIGHT(i) (2*i+1)
#define INFINITY 9999
#define N 5
#define S 0
#define D 4

/*
	Implement Priority Queue with min heap has maked in 1st year at Data Structure Course	
	We modify the Priority Queue to 2d matrix that hold at [1] index of vertex,
	and at [0] the weight from source vertex.
						*/
void min_heapify(int heap[][2], int i, int n) {
    int l = HEAP_LEFT(i);
    int r = HEAP_RIGHT(i);
    int smallest = 0;
    
    if(l <= n && heap[l-1][0] < heap[i-1][0]) {
        smallest = l;
    } else {
        smallest = i;
    }
    if(r <= n && heap[r-1][0] < heap[smallest-1][0]) {
        smallest = r;
    }
    if(smallest != i) {
        int temp1 = heap[i-1][0];
        int temp2 = heap[i-1][1];
        
        heap[i-1][0] = heap[smallest-1][0];
        heap[i-1][1] = heap[smallest-1][1];
        heap[smallest-1][0] = temp1;
        heap[smallest-1][1] = temp2;

        min_heapify(heap, smallest, n);
    }
}
//build a min heap,calling to min_heapify
void build_min_heap(int heap[][2],int n) {    
    int i;
	for(i = (n/2); i > 0; i--) {
        min_heapify(heap, i, n);
    }
}
//pass the vertex index and return the min value of her
int heap_extract_min(int heap[][2],int n,int *vertexIndex) { 
    int min = heap[0][0];
    int u = heap[0][1];
    if(n > 1) {        
        heap[0][0] = heap[n-1][0];
        heap[0][1] = heap[n-1][1];
        build_min_heap(heap, n - 1);
    }
    *vertexIndex= u;
    return min;
}
void heap_decrease_key(int heap[][2],int u,int weight,int n) {
    int i,parent,temp1,temp2;
	for(i = 0; i < n; i++) {
        if(heap[i][1] == u) {
            if(weight < heap[i][0]) {
                heap[i][0] = weight;
                parent = HEAP_PARENT(i);
                while(i > 0 && heap[parent][0] > heap[i][0]) {
                    //swap
                    temp1 = heap[parent][0];
                    temp2 = heap[parent][1];
                    heap[parent][0] = heap[i][0];
                    heap[parent][1] = heap[i][1];
                    heap[i][0] = temp1;
                    heap[i][1] = temp2;
                    i = parent;
                    parent = HEAP_PARENT(i);
                }
            }
            break;
        }
    }

}
//print heap for testing uses
void print_heap(int h[][2]){
	int i;
	for (i =0; i < N; i++ ){
		printf("%d/%d ",h[i][0],h[i][1]);
	}
	printf("\n");
}

//Full disclosure: we tried a lot to found a way to print the path from the father array with out success,
//so we google it and take this print method from GeekToGeeks.com(all the other code is own)
void printPath(int parent[], int j)
{
    // Base Case : If j is source
    if (parent[j]==-1)
        return;
    printPath(parent, parent[j]);
 
    printf("%d ", j);
}
/*
	Dijkstra algorithm:
	The code algorithm below work with directed and undirected graph,
	only if the weight is non-negative.

	heap[][] => Priorty queue [0]=>weight of veretx from  source,[1]=>vertex index
	visited[] => by lectures ,it hold the visited/not visited vertex by index
	dist[] => hold the weight of vertex from the source
	distEdges[] => hold the sum of edges from te soruce
	path[] => fathers array by the min weight and edges
	*/
void dijkstra(int graph[][N], int n,int source,int dest) {
    int heap[N][2],visited[N],distWeight[N],distEdges[N];
	int i,u,min_val;
	int	w,v;
	int	weight,edges;
	int heap_size = n;
	int path[N];
	//init arrays
    for(i = 0; i < n; i++) {
        distWeight[i] = INFINITY;
		distEdges[i] = 0;
        heap[i][0] = INFINITY;
        heap[i][1] = i;
        visited[i] = 0;
		path[i] = -1;
    }
	//init the source vertex
	path[source] = -1;
    distWeight[source] = 0;
	distEdges[source] = 0;
    heap[source][0] = 0;
	//build the queue
    build_min_heap(heap, heap_size);
    while(heap_size > 0) {
        u = -1;
        min_val = heap_extract_min(heap, heap_size, &u);
		visited[u] = 1;//visited
        --heap_size;
        for (v = 0; v < n; v++) {
			w = graph[u][v];
            if (w !=INFINITY && visited[v] != 1 ) {
				edges = distEdges[u] + 1;
				weight = distWeight[u] + w;
				//the modification ,if the weights equal and the number of edges of the father small than v
				if( weight == distWeight[v] && edges < distEdges[v]){
					distEdges[v] = edges;
					printf("test:%d",edges);
					path[v] = u;
				}//end modification
				else if(distWeight[v] > weight) {
					distWeight[v] = weight; 
					distEdges[v] = edges;
					heap_decrease_key(heap, v, weight, n);
					path[v] = u;
				}
            }
        }
	}
	
	printf("Vertex\t  Distance\tPath");
    printf("\n%d -> %d \t\t %d\t\t%d ", source, dest, distWeight[dest], source);
    printPath(path, dest);
	printf("\n");
	
}

int main(){
   
	//the example graph,regualr dijkstra return the same path(with or without the modification
	int graph[N][N] = {{0,6,5,1,INFINITY},{6,0,3,INFINITY,INFINITY},
	{5,3,0,2,5},{1,INFINITY,2,0,6},{INFINITY,INFINITY,5,6,0}};
	//DR.Eugene example to graph, that regualr dijkstra return the LONG edges path WITHOUT the modification 
	int graph1[N][N] = {{0,6,10,100,INFINITY},
	{6,0,INFINITY,1,INFINITY},
	{10,INFINITY,0,5,2},
	{100,1,5,0,5},
	{INFINITY,INFINITY,2,5,0}};


    dijkstra(graph1, N,S,D);
	getchar();
	
    return 0;
}
