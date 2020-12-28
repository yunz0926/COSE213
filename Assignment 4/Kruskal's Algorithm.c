#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ELEMENTS 200
#define HEAP_FULL(n) (n == MAX_ELEMENTS-1)
#define HEAP_EMPTY(n) (!n)

/*structure for presenting edges*/
typedef struct {
	int v; //node1
	int w; //node2
	int cost; //weight of the edge
}element;

element heap[MAX_ELEMENTS]; //min-heap for storing edges with weight
element spanningtree[MAX_ELEMENTS]; 
int parent[100]; //array for storing a parent of a node
int n = 0;

void push(element item);
element pop();
int Find(int i);
void Union(int i, int j);

int main(int argc, char *argv[])
{
	memset(parent, -1, sizeof(parent)); //initialize the parent of each node to itself

	FILE * input = fopen(argv[1], "r");

	int edge, node, v, w, cost;
	int tredge = 0;
	element e;
	fscanf(input, "%d %d", &node, &edge);

	for (int i = 0; i < edge; i++)
	{
		fscanf(input,"%d %d %d", &v, &w, &cost);
		element item = { v, w, cost }; //make new edge
		push(item); //push the item to min-heap
	}
	fclose(input);
	int f1, f2;

	while (tredge < node - 1 && !HEAP_EMPTY(n)) 
		//Tree contains less than n-1 edges && heap is not empty
	{
		e = pop();
		if ((f1 = Find(e.v)) != (f2 = Find(e.w))) 
			//if parent of e.v and parent of e.w are not same -> (e.v, e.w) doesn't create a cycle
		{
			spanningtree[tredge++] = e; //add (e.v, e.w) to Tree
			Union(f1, f2); //combine a tree with f1 as root and a tree with f2 as root
		}
	}

	if (tredge != node - 1) //if tree contains fewer than n-1 edges
	{
		printf("No spanning tree...\n");
		return 0; //exit the program
	}
	
	FILE * output = fopen(argv[2], "w");

	for (int i = 0; i < tredge; i++)
	{
		fprintf(output, "%d %d\n", spanningtree[i].v, spanningtree[i].w);
		//print edges of minimum cost spanning tree
	}

	fclose(output);
}

void push(element item)
{
	int i;
	if (HEAP_FULL(n))
	{
		fprintf(stderr, "The heap is full. \n");
		exit(EXIT_FAILURE);
	}
	i = ++n;
	while ((i != 1) && (item.cost < heap[i / 2].cost))
	{
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}

element pop()
{
	int parent, child;
	element item, temp;

	if (HEAP_EMPTY(n))
	{
		fprintf(stderr, "The heap is empty\n");
		exit(EXIT_FAILURE);
	}
	item = heap[1];
	temp = heap[n--];
	parent = 1;
	child = 2;
	while (child <= n)
	{
		if (child < n && heap[child].cost > heap[child + 1].cost)
			child++;
		if (temp.cost <= heap[child].cost) break;
		heap[parent] = heap[child];
		parent = child;
		child *= 2;
	}
	heap[parent] = temp;
	return item;
}

int Find(int i)
{
	for (; parent[i] >= 0; i = parent[i]);
	return i;
}

void Union(int i, int j)
{
	int temp = parent[i] + parent[j];
	if (parent[i] > parent[j])
	{
		parent[i] = j;
		parent[j] = temp;
	}
	else
	{
		parent[j] = i;
		parent[i] = temp;
	}
}