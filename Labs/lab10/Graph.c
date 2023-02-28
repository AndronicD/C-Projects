#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Util.h"
#include "Graph.h"

TGraphL* createGraphAdjList(int numberOfNodes) {
	//TODO: 1

	TGraphL *graph = (TGraphL *) malloc(sizeof(TGraphL));

	graph->nn = numberOfNodes;
	graph->adl =(TNode **)malloc(numberOfNodes * sizeof(TNode *));
	for(int i = 0; i  < numberOfNodes; i++){
		graph->adl[i] = NULL;
	}

	return graph;
}

void addEdgeList(TGraphL* graph, int v1, int v2) {
	//TODO: 1

	TNode *n1, *n2;
	n1 = (TNode *)malloc(sizeof(TNode));
	n2 = (TNode *) malloc(sizeof(TNode));

	n1->v = v2;
	n1->c = 0;
	n1->next = graph->adl[v1];
	graph->adl[v1] = n1;

	n2->v = v1;
	n2->c = 0;
	n2->next = graph->adl[v2];
	graph->adl[v2] = n2;
}

List* dfsIterative(TGraphL* graph, int s) {

	//TODO: 2

	Stack *stack = createStack();
	int *visited = (int *)malloc(graph->nn * sizeof(int));

	for(int i = 0 ; i < graph->nn; i++)
		visited[i] = 0;

	List *path = createList();

	push(stack, s);
	visited[s] = 1;

	while(!isStackEmpty(stack)){
		s = top(stack);
		pop(stack);

		enqueue(path, s);

		for(TNode* it = graph->adl[s]; it!= NULL; it = it->next){
			if(!visited[it->v]){
				visited[it->v] = 1;
				push(stack, it->v);
			}
		}
		
	}
	destroyStack(stack);
	free(visited);
	return path;

}

void dfsRecHelper(TGraphL* graph, int* visited, List* path, int s) {
	//TODO: 3
	visited[s] = 1;
	enqueue(path, s);

	for(TNode* it = graph->adl[s]; it != NULL; it = it->next){
		if(visited[it->v] != 1)
			dfsRecHelper(graph, visited, path, it->v);
	}
}

List* dfsRecursive(TGraphL* graph, int s) {
	// TODO 3
	List *path = createList();
	int *visited = (int *)malloc(graph->nn * sizeof(int));

	for(int i = 0 ; i < graph->nn; i++)
		visited[i] = 0;

	dfsRecHelper(graph, visited, path, s);

	free(visited);
	return path;
}

List* bfs(TGraphL* graph, int s){
	// TODO: 4

	Queue *queue = createQueue();
	int *visited = (int *)malloc(graph->nn * sizeof(int));

	for(int i = 0 ; i < graph->nn; i++)
		visited[i] = 0;

	List *path = createList();

	enqueue(queue, s);
	visited[s] = 1;

	while(!isQueueEmpty(queue)){
		s = front(queue);
		dequeue(queue);

		enqueue(path, s);

		for(TNode* it = graph->adl[s]; it!= NULL; it = it->next){
			if(!visited[it->v]){
				visited[it->v] = 1;
				enqueue(queue, it->v);
			}
		}
		
	}
	destroyQueue(queue);
	free(visited);
	return path;

}


void destroyGraphAdjList(TGraphL* graph){
	for(int i = 0; i < graph->nn; i++){
		while(graph->adl[i] != NULL){
			TNode* aux = graph->adl[i];
			graph->adl[i] = graph->adl[i]->next;
			free(aux);
		}
	}
	free(graph->adl);

	free(graph);
}

void removeEdgeList(TGraphL* graph, int v1, int v2){
	TNode* it = graph->adl[v1];
	TNode* prev = NULL;
	while(it != NULL && it->v != v2){
		prev = it;
		it = it->next;
	}

	if(it == NULL)return;

	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v1] = it->next;
	free(it);

	it = graph->adl[v2];
	prev = NULL;
	while(it != NULL && it->v != v1){
		prev = it;
		it = it->next;
	}
	if(it == NULL) return;
	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v2] = it->next;

	free(it);
}

void removeNodeList(TGraphL* graph, int v){
	for(int i = 0; i < graph->nn;i++){
		removeEdgeList(graph,v,i);
	}
}
