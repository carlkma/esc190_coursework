#include "lab5.h"

int get_vertex_index_by_name(Graph *gr, char* name){

    int count_vertices = gr->count;
    Vnode **arr_vertices = gr->adj_list;

    for (int i=0; i<count_vertices; i++){
        if (strcmp(arr_vertices[i]->station, name) == 0) return i;
    }
    return -1;
}

int get_vertex_index_by_lowest_cost(Graph *gr){

    int count_vertices = gr->count;
    Vnode **arr_vertices = gr->adj_list;
    
    int temp_min = INT_MAX;
    int temp_min_index = -1;

    for (int i=0; i<count_vertices; i++){
        if (arr_vertices[i]->visited == 1) continue;
        if (arr_vertices[i]->cost < temp_min){
            temp_min = arr_vertices[i]->cost;
            temp_min_index = i;
        }
    }
    return temp_min_index;

}

char **plan_route(Graph *gr, char *start, char *dest){
    
    // define local var from Graph *gr
    int count_vertices = gr->count;
    Vnode **arr_vertices = gr->adj_list;

    // define output array of strings
    char **output_sequence;
    // max length of seq is count_vertices 
    output_sequence = (char**) malloc(sizeof(char*) * count_vertices);

    int start_index = -1;
    int dest_index = -1;

    // loop thru adj_list once, init optional var
    for (int i=0; i<count_vertices; i++){
        if (strcmp(arr_vertices[i]->station, start) == 0) start_index=i; 
        if (strcmp(arr_vertices[i]->station, dest) == 0) dest_index=i;

        arr_vertices[i]->visited = 0;
        arr_vertices[i]->prev = NULL;
        arr_vertices[i]->cost = INT_MAX;
    }

    // simple check to verify existence of start and dest
    if (start_index==-1 || dest_index==-1) return NULL;

    // cost of start node is 0
    arr_vertices[start_index]->cost = 0;

    int current_index = start_index;

    // loop thru adj_list once (until every vertex is visited) 
    while (current_index != -1){
        
        // set visited
        arr_vertices[current_index]->visited = 1;

        // define array of edges (aka starting node of linked list)
        Enode *arr_edges = arr_vertices[current_index]->edges;

        // nested loop: determine cost from current node to every adjacent node
        while (arr_edges != NULL){

            // get name of adjacent node
            char *temp_vertex_name = arr_edges->vertex;

            // get index of adjacent node in the primary array, adj_list
            int temp_vertex_index = get_vertex_index_by_name(gr, temp_vertex_name);

            // in case adjacent node is not in adj_list
            if (temp_vertex_index==-1) return NULL;

            // modify cost associated with adjacent node, if appropriate
            int old_cost = arr_vertices[temp_vertex_index]->cost;
            int new_cost = arr_vertices[current_index]->cost + arr_edges->weight;

            if (new_cost < old_cost){
                arr_vertices[temp_vertex_index]->cost = new_cost;
                arr_vertices[temp_vertex_index]->prev = arr_vertices[current_index];
            }

            // moving on to another adjacent node
            arr_edges = arr_edges->next;
        }

        // examine a new un-visited vertex
        current_index = get_vertex_index_by_lowest_cost(gr);
    }

    // adj_list is now complete with data

    int out_vertex_index = dest;
    int end = 0;
    for (int out_index=0; out_index<count_vertices; out_index++){
        if (end==0){
            output_sequence[out_index] = arr_vertices[out_vertex_index]->station;
        }
        else{
            return output_sequence;
        }
        if (arr_vertices[out_vertex_index]->prev != NULL){
            out_vertex_index = get_vertex_index_by_name(gr, arr_vertices[out_vertex_index]->prev->station);
        }
        else{
            end = 1;
        }
    return NULL;

    }

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

void update(Graph *gr, char *start, char *dest, int weight){

    // no action if weight does not exist
    if (weight == NULL) return;

    // add start and/or dest to graph is nonexistent
    if (get_vertex_index_by_name(gr, start) == -1){
        add(gr, start);
    }
    if (get_vertex_index_by_name(gr, dest) == -1){
        add(gr, dest);
    }

    // define local var from Graph *gr
    int count_vertices = gr->count;
    Vnode **arr_vertices = gr->adj_list;

    int start_index = get_vertex_index_by_name(gr, start);
    Enode *arr_edges = arr_vertices[start_index]->edges;

    // nested loop: 
    while (arr_edges != NULL){
        if (strcmp(arr_edges->vertex,dest) == 0){
            arr_edges->weight = weight;
            return;
        }
        arr_edges = arr_edges->next;
    }
    Enode *new_enode;
    new_enode = (Enode*) malloc(sizeof(Enode));
    strcpy(new_enode->vertex, dest);
    new_enode->weight = weight;
    new_enode->next = NULL;
    
    arr_edges = arr_vertices[start_index]->edges;
    while (arr_edges->next != NULL) arr_edges = arr_edges->next;
    arr_edges->next = new_enode;
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

void disrupt(Graph *gr, char *station){
    
    // no action if station does not exist
    if (get_vertex_index_by_name(gr, station) == -1) return;

    // define local var from Graph *gr
    int count_vertices = gr->count;
    Vnode **arr_vertices = gr->adj_list;

    // get index of vertex to delete
    int del_index = get_vertex_index_by_name(gr, station);

    // remove edge from this node to adjacent nodes
    // aka remove this node from arr_vertices and free
    Enode *edge_node_arr_to_free = arr_vertices[del_index]->edges;
    free_linked_list(edge_node_arr_to_free);

    // shift remaining list items one step forward
    gr->count = (gr->count) - 1;
    count_vertices = gr->count;
    for (int i=del_index; i<count_vertices; i++){
        arr_vertices[del_index] = arr_vertices[del_index+1];
    }

    // remove edge from other nodes to this node
    // aka loop thru all other nodes
    for (int i=0; i<count_vertices; i++){
        Enode *edge_node_arr_to_check = arr_vertices[i]->edges;
        arr_vertices[i]->edges = delete_node(edge_node_arr_to_check, station);

    }
    
}

int main(){
    // to be removed
}