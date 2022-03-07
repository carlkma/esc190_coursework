// Linked List Implementation of Queue

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node {
    char* name;
    int value;
    struct node *next; 
} node;

void enqueue(node **first, char* a_name, int a_value) {

    // create new node
    node *new_node;
    new_node = (node*) malloc(sizeof(node));
    // assigning a_name via strcpy
    new_node->name = (char*) malloc( sizeof(char) * (strlen(a_name) +1));
    strcpy(new_node->name,a_name);
    new_node->value = a_value;
    new_node->next = NULL;
    
    if (*first == NULL) {
        *first = new_node;
        return;
    }
    node *current;
    current = *first;
    while (current->next != NULL) current = current->next;
    current->next = new_node;

    return;
}

char* dequeue(node **first){
    
    char* output_name;

    if (*first == NULL) return NULL;

    if ((*first)->next == NULL){
        output_name = (*first)->name;
        free(*first);
    }
    else {

        node *tmp_ptr_to_first = *first;
        output_name = (*first)->name;
        *first  = (*first)->next;
        free(tmp_ptr_to_first);
    }
    return output_name;
}

void print_linked_list(struct node *first) {
    node *current = first;
    
    printf("{");
    
    while (current != NULL && current->next != NULL) {
        
        printf("%s: ", current->name);
        printf("%d, ", current->value);
        current = current->next;
    }
    
    printf("%s: %d}\n", current->name, current->value);
    
    return;
}


int main() {
    
    node *head = NULL;
    
    enqueue(&head,"large",10);
    enqueue(&head,"large",100);
    enqueue(&head,"small",1);
    enqueue(&head,"medium",5);
    print_linked_list(head);

    char* result = dequeue(&head);
    result = dequeue(&head);
    printf("%s\n", result);
    print_linked_list(head);


    return 0;
}