#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#include "os_graph.h"
#include "os_threadpool.h"
#include "os_list.h"

#define MAX_TASK 100
#define MAX_THREAD 4

int sum = 0;
os_graph_t *graph;
os_threadpool_t *tp;
pthread_mutex_t graphLock;

int first_unvisited(){
    int i = 0;
    int stop = 0;
    while(i < graph->nCount){
        if(!graph->visited[i]){
            stop = i;
            break;
        }
        i++;
    }
    if(i == graph->nCount){
        return -1;
    }
    return stop;
}

void processNode(void *node)
{
    pthread_mutex_lock(&graphLock);
    if(node != NULL){
        os_node_t *n = (os_node_t *)node;
        if (n != NULL) {
            sum += n->nodeInfo;
            int count = 0;
            for(int i = 0; i < n->cNeighbours; i++){
                if (graph->visited[n->neighbours[i]] == 0) {
                    count++;
                    os_node_t *node = graph->nodes[n->neighbours[i]];
                    os_task_t *task = task_create((void *)node, processNode);
                    add_task_in_queue(tp, task);
                    graph->visited[n->neighbours[i]] = 1;
                }
            }

            if(count == 0){
                int rand = first_unvisited();
                if(rand != -1){
                    os_node_t *node = graph->nodes[rand];
                    os_task_t *task = task_create((void *)node, processNode);
                    add_task_in_queue(tp, task);
                    graph->visited[rand] = 1;
                }
            }
        }
    }
    pthread_mutex_unlock(&graphLock);
}

int processingIsDone(os_threadpool_t *tp)
{
    /* Stop processing nodes if all tasks are complete */
    int i = 0;
    int stop = 1;
    while(i < graph->nCount){
        if(!graph->visited[i]){
            stop = 0;
            break;
        }
        i++;
    }
    return stop && (tp->tasks == NULL);
}

void free_graph(os_graph_t *graph) {
    if (graph == NULL) {
        return;
    }

    for (int i = 0; i < graph->nCount; i++) {
        os_node_t *node = graph->nodes[i];
        if (node != NULL) {
            if (node->neighbours != NULL) {
                free(node->neighbours);
            }
            free(node);
        }
    }

    free(graph->nodes);
    free(graph->visited);
    free(graph);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./main input_file\n");
        exit(1);
    }

    FILE *input_file = fopen(argv[1], "r");

    if (input_file == NULL) {
        printf("[Error] Can't open file\n");
        return -1;
    }

    graph = create_graph_from_file(input_file);
    if (graph == NULL) {
        printf("[Error] Can't read the graph from file\n");
        return -1;
    }

    // TODO: create thread pool and traverse the graf
    /* Create the thread pool */
    pthread_mutex_init(&graphLock, NULL);
    tp = threadpool_create(graph->nCount, MAX_THREAD);

    /* Add all nodes as tasks to the thread pool task queue */

    os_node_t *node = graph->nodes[0];
    graph->visited[0] = 1;
    os_task_t *task = task_create((void *)node, processNode);
    add_task_in_queue(tp, task);

    /* Wait for all tasks to complete */
    threadpool_stop(tp, &processingIsDone);

    /* Clean up thread pool and graph resources */
    pthread_mutex_destroy(&graphLock);
    pthread_mutex_destroy(&(tp->taskLock));
    free_graph(graph);
    free(tp->threads);
    free(tp);

    printf("%d", sum);
    return 0;
}
