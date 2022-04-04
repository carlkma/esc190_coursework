#include "lab5.h"

//Makes a pointer to a Graph struct
Graph *initialize_graph(){
	Graph *g = malloc(sizeof(Graph));
	g->adj_list = NULL;
	g->count = 0;
	return g;
}

//Prints a Graph's vertex count, vertices, edges, and edge weights. 
void print_gr(Graph* gr){
	Enode *curr=NULL;
    printf("Printing the graph with count %d...\n", gr->count);
	for (int i=0; i<gr->count; i++){
		printf("%s station: ", gr->adj_list[i]->station);
        curr = gr->adj_list[i]->edges;
        while(curr!=NULL){
            printf("%s(%d) ", curr->vertex, curr->weight);
            curr = curr->next;
        }
        printf("\n");
    }
	printf("\n");
}

//Free memory allocated for a Graph
void free_gr(Graph* gr){
    char station[MAX_LEN];
    while(gr->adj_list != NULL){
        strcpy(station, gr->adj_list[0]->station);
        disrupt(gr, station);
    }
    free(gr);
    gr = NULL;
    return;
}

//Print (and free) the shortest path from plan_route()
//Must provide the correct start vertex
void print_route(char **route, char *start){
    if (route){
        int count = 0;
        printf("The shortest path, tracing backwards, is: ");
        char *curr = route[count];
        while(strcmp(curr, start)!=0){
            printf("%s, ", curr);
            free(curr);
            count++;
            curr = route[count];
        }
        printf("%s.\n\n", curr);
        free(curr);
        free(route);
    }
    else{
        printf("No path was found :(\n\n");
    }
}