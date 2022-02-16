#include "lab3.h"
#include <string.h>

//Add TA party item request to the list
int add_request(struct party_node** head, char *item, double price, char *ta){  
    if (strcmp(item,"IDE") != 0){
    struct party_node* new_node = (struct party_node*) malloc(sizeof(struct party_node));
    new_node->item = (char*) malloc( sizeof (char) * (strlen(item) +1));
    //new_node->item = item; //str copy
    strcpy(new_node->item,item);
    new_node->price = price;
    new_node->ta = (char*) malloc( sizeof (char)* (strlen(ta) +1));

    //new_node->ta = ta;
    strcpy(new_node->ta, ta);
    new_node->next = *head;
    *head = new_node;
    return 0;
    }
    
    else {
        return -1;
    }


}

//Remove the last item added
void remove_request(struct party_node **head){

    if (*head == NULL){
        return; // remove request when stack is empty
    }
    struct party_node* to_remove= *head;
    struct party_node* next = to_remove->next;
    free(to_remove->item);
    free(to_remove->ta);
    free(to_remove);
    *head = next;


}

//Sort party item requests - in place?
void make_sorted(struct party_node **head){
    struct party_node* big_loop = *head;
    struct party_node* node = NULL;
    
    
    while (big_loop != NULL) {
        node = *head;
        while (node->next != NULL){
            if (node->price < node->next->price){

                char* item1 = node->item;
	            double price1 = node->price;
                char* ta1 = node->ta;
                
                node->item = node->next->item;
                node->price = node->next->price;
                node->ta = node->next->ta;

                node->next->item = item1;
                node->next->price = price1;
                node->next->ta = ta1;

            }
            node = node->next;
        }
        big_loop = big_loop->next;
    }
}

//Trim list to fit the budget
double finalize_list(struct party_node **head, double budget){
    struct party_node* current = *head;
    struct party_node* pre = *head;
    
    
    while (current != NULL) {
        double cost = current->price;
        if (cost > budget){
            // case 1, first item
                // header point to the second item directly

            // case 2, subsequent items
                // pre.next = current.next
                // free(current)
            if (current == *head){
                *head = current->next;
                free(current->item);
                free(current->ta);
                free(current);
                current = *head;
            }
            else {
                pre->next = current->next;
                free(current->item);
                free(current->ta);
                free(current);
                current = current->next;


            }
        }
        else {
            budget = budget - cost;
            pre = current;
            current = current->next;

        }
    }
    return budget;
}

//Print the current list - hope this is helpful!
void print_list(struct party_node *head){
    int count = 1;
    printf("The current list contains:\n");
    while(head!=NULL){
        printf("Item %d: %s, %.2lf, requested by %s\n",
            count, head->item, head->price, head->ta);
        count++;
        head = head->next;
    }
    printf("\n\n");
    return;
}