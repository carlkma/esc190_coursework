// Test
// gcc -o test test.c ; ./test

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



struct node {
    int value;
    struct node *next;
};

void insert_node(struct node **first, int val) {
    struct node *new_node;
    new_node = (struct node*) malloc(sizeof(struct node));
    new_node->value = val;
    new_node->next = *first;
    *first = new_node;
    return;
}

void delete_node(struct node **first) {
    struct node *temp_first = *first;
    *first = (*first)->next;
    free(temp_first);
    return;
}

void delete_node_at_index(struct node *first, int idx) {
    if (idx==0) printf("use delete_node function");
    struct node *previous = NULL;
    struct node *current = first;
    
    
    for (int i=0 ; i<idx ; i++) {
        previous = current;
        current = current->next;
        if (current == NULL) {
            printf("index out of range\n");
            return;
        }
    }

    previous->next = current->next;
    free(current);
    return;
}

void insert_node_at_index(struct node *first, int val, int idx) {
    // undefined behavior for idx=0
    if (idx==0) printf("use insert_node function");
    struct node *new_node;
    new_node = (struct node*) malloc(sizeof(struct node));
    new_node->value = val;

    struct node *previous = NULL;
    struct node *current = first;
    
    
    for (int i=0 ; i<idx ; i++) {
        previous = current;
        current = current->next;
        if (current == NULL && i+1<idx) {
            printf("index out of range\n");
            return;
        }
    }

    new_node->next = current;
    previous->next = new_node;

    return;
}

void print_linked_list(struct node *first) {
    struct node *current = first;
    printf("{");
    while (current != NULL && current->next != NULL) {
        printf("%d, ", current->value);
        current = current->next;
    }
    printf("%d}\n", current->value);
    return;
}

int main() {
    
    struct node *first = NULL;
    
    insert_node(&first, 10);
    insert_node(&first, 20);
    insert_node(&first, 30);
    insert_node(&first, 40);

    print_linked_list(first);
    insert_node_at_index(first, 1111, 4);
    //delete_node_at_index(first,3);
    print_linked_list(first);

}

