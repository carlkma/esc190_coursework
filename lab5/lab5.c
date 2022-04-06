/*----------------------------------------
ESC190 Lab 5
Apr 6, 2022

Ma, Carl Ka To
Xu, Shenxiaozhu

Test submission, to be updated
----------------------------------------*/

#include "lab5.h"
#define INT_MAX 2147483647


int get_vertex_index_by_name(Graph *gr, char* name){
    // helper function to get index of vertex <name> in gr->adj_list
    // loop thru gr->adj_list, return index if name matches 

    for (int i=0; i<(gr->count); i++){
        if (strcmp((gr->adj_list)[i]->station, name) == 0) return i;
    }
    return -1; // -1 if nonexistent
}


int get_vertex_index_by_lowest_cost(Graph *gr){
    // helper function to get index of vertex with lowest cost in gr->adj_list
    // loop thru gr->adj_list, ignore visited vertices

    int temp_min = INT_MAX;
    int temp_min_index = -1;

    for (int i=0; i<(gr->count); i++){
        if ((gr->adj_list)[i]->visited == 1) continue;
        if ((gr->adj_list)[i]->cost < temp_min){
            temp_min = (gr->adj_list)[i]->cost;
            temp_min_index = i;
        }
    }
    return temp_min_index;
}


char **plan_route(Graph *gr, char *start, char *dest){

    // define output array of strings
    char **output_sequence;
    // max length of seq is (gr->count)
    output_sequence = (char**) calloc(sizeof(char*), (gr->count));

    int start_index = -1;
    int dest_index = -1;


    // loop thru adj_list once, init optional var
    for (int i=0; i<(gr->count); i++){
        if (strcmp((gr->adj_list)[i]->station, start) == 0) start_index=i; 
        if (strcmp((gr->adj_list)[i]->station, dest) == 0) dest_index=i;

        (gr->adj_list)[i]->visited = 0;
        (gr->adj_list)[i]->prev = NULL;
        (gr->adj_list)[i]->cost = INT_MAX;
    }

    // simple check to verify existence of start and dest
    if (start_index==-1 || dest_index==-1) return NULL;

    // cost of start node is 0
    (gr->adj_list)[start_index]->cost = 0;

    int current_index = start_index;

    // loop thru adj_list once (until every vertex is visited) 
    while (current_index != -1){
        
        // set visited
        (gr->adj_list)[current_index]->visited = 1;

        // define array of edges (aka starting node of linked list)
        Enode *arr_edges = (gr->adj_list)[current_index]->edges;

        // nested loop: determine cost from current node to every adjacent node
        while (arr_edges != NULL){

            // get name of adjacent node
            char *temp_vertex_name = arr_edges->vertex;
            //printf("name of current %s\n", (gr->adj_list)[current_index]->station);
            //printf("name of adj %s\n",temp_vertex_name);

            // get index of adjacent node in the primary array, adj_list
            int temp_vertex_index = get_vertex_index_by_name(gr, temp_vertex_name);

            // in case adjacent node is not in adj_list
            if (temp_vertex_index==-1) return NULL;

            // modify cost associated with adjacent node, if appropriate
            int old_cost = (gr->adj_list)[temp_vertex_index]->cost;
            int new_cost = (gr->adj_list)[current_index]->cost + arr_edges->weight;

            //printf("%d\n",new_cost);
            //printf("%d\n",old_cost);

            if (new_cost < old_cost){
                (gr->adj_list)[temp_vertex_index]->cost = new_cost;
                (gr->adj_list)[temp_vertex_index]->prev = (gr->adj_list)[current_index];
            }

            // moving on to another adjacent node
            arr_edges = arr_edges->next;
        }

        // examine a new un-visited vertex
        current_index = get_vertex_index_by_lowest_cost(gr);
    }

    // adj_list is now complete with data
    

    int end = 0;
    for (int out_index=0; out_index<=(gr->count); out_index++){
        if (end==0){
            char *to_add;
            to_add = (char*) malloc(sizeof(char) * MAX_LEN);
            strcpy(to_add, (gr->adj_list)[dest_index]->station);
            (output_sequence[out_index]) = to_add;
        }
        else{
            if (strcmp(start, (output_sequence[out_index-1]))==0) return output_sequence;
            else return NULL;
        }
        if ((gr->adj_list)[dest_index]->prev != NULL){
            dest_index = get_vertex_index_by_name(gr, (gr->adj_list)[dest_index]->prev->station);
        }
        else{
            end = 1;
        }
    }
    return NULL;
}

void add(Graph *gr, char *station){

    // no action if station exists
    if (get_vertex_index_by_name(gr, station) != -1) return;

    // create new_vnode
    Vnode *new_vnode;
    new_vnode = (Vnode*) malloc(sizeof(Vnode));

    strcpy(new_vnode->station, station);
    new_vnode->edges = NULL;

    gr->adj_list = realloc(gr->adj_list, sizeof(Vnode*) * ( (gr->count) + 1) );
    (gr->adj_list)[gr->count] = new_vnode;
    gr->count = ( (gr->count) + 1);

}


Enode* delete_node(Enode *head, char *to_delete){
    Enode *curr=head, *prev=NULL;

    //stop if we get to the end or find the node to delete
    while(curr!=NULL && strcmp(curr->vertex,to_delete)!=0){
        prev = curr;        //advance both pointers
        curr = curr->next;
    }
    if (curr==NULL){         //couldn't find it
        return head;
    }   
    else if (prev==NULL){   //deleting first node
        head = head->next;  //safe because curr is pointing to first node
    }
    else{                   //deleting non-first node
        prev->next = curr->next; //bypass
    }
    free(curr);
    return head;
}

void update(Graph *gr, char *start, char *dest, int weight){

    int start_index = get_vertex_index_by_name(gr, start);
    int dest_index = get_vertex_index_by_name(gr, dest);
    

    // case 1: weight==0
    if (weight==0){
        // case 1.1: start and/or dest nonexistent
        if (start_index==-1 || dest_index==-1) return;
        // case 1.2: start and dest exist, remove edge from start to dest
        (gr->adj_list)[start_index]->edges = delete_node((gr->adj_list)[start_index]->edges, dest);
        return;
    }

    // case 2: weight!=0
    // add start and/or dest to graph if nonexistent

    if (start_index == -1 ){
        add(gr, start);
        start_index = (gr->count) - 1;
        
    }
    if (dest_index == -1){
        add(gr, dest);
        dest_index = (gr->count) - 1;
        

    }
    Enode *temp_edge = gr->adj_list[start_index]->edges;
    
    
    // modify existing weight value
    

    while (temp_edge != NULL){
        if (strcmp(temp_edge->vertex,dest) == 0){
            temp_edge->weight = weight;
            return;
        }
        temp_edge = temp_edge->next;
    }

    // create new edge
    
    Enode *new_enode;
    new_enode = (Enode*) malloc(sizeof(Enode));
    strcpy(new_enode->vertex, dest);
    new_enode->weight = weight;
    new_enode->next = NULL;
    
    temp_edge = gr->adj_list[start_index]->edges;
   

    // new edge is first edge
    if (temp_edge == NULL){
        gr->adj_list[start_index]->edges = new_enode;
        return;
    }

    // new edge is subsequent edge
    while (temp_edge->next != NULL) temp_edge = temp_edge->next;
    
    temp_edge->next = new_enode;
    return;
}

void free_linked_list(Enode *head){
    Enode *temp;
    while (head!=NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
}



void disrupt(Graph *gr, char *station){
    
    // no action if station does not exist
    if (get_vertex_index_by_name(gr, station) == -1) return;

    // get index of vertex to delete
    int del_index = get_vertex_index_by_name(gr, station);

    // remove edge from this node to adjacent nodes
    // aka remove this node from (gr->adj_list) and free
    Enode *edge_node_arr_to_free = (gr->adj_list)[del_index]->edges;
    free_linked_list(edge_node_arr_to_free);
    free((gr->adj_list)[del_index]);

    // shift remaining list items one step forward
    gr->count = (gr->count) - 1;

    // nothing left
    if (gr->count == 0){
        gr->adj_list = NULL;
        return;
    }

    for (int i=del_index; i<(gr->count); i++){
        (gr->adj_list)[i] = (gr->adj_list)[i+1];
    }

    // remove edge from other nodes to this node
    // aka loop thru all other nodes
    for (int i=0; i<(gr->count); i++){
        Enode *edge_node_arr_to_check = (gr->adj_list)[i]->edges;
        (gr->adj_list)[i]->edges = delete_node(edge_node_arr_to_check, station);

    }
    
}
