/* --------------------------------------------------------------------------------

// ESC190: Computer Algorithms and Data Structures
// Assignment 1
// by Carl Ma and Shenxiaozhu Xu
// March 30, 2022

-------------------------------------------------------------------------------- */

#include "a1.h" // header file provided with the assignment, unmodified

// "precision (of the double item_cost_per_unit) is not guaranteed"
// hence, the standard maximum precision of a double type is assumed
#define MAX_PRECISION_DOUBLE 16


// helper function to determine if char c is a whitespace character
// input: unsigned char c
// output: boolean (int) 0 or 1
int a_isspace(unsigned char c) {
	const char whitespace[] = " \n\t\v\r";
	if (strchr(whitespace, c) == NULL) return 0; // is c not in "whitespace"?
	return 1;
}


// helper function to trim leading and trailing whitespaces
// input: string str - a pointer to char
// output: string new_str - a pointer to char
// note: this function is adapted from stackoverflow
char *trimwhitespace(char *str) {
  char *end;

  // Trim leading space
  while(a_isspace((unsigned char)*str)) str++;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && a_isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}


/* --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
FIRST TASK: Initialize Restaurant

Memory Allocation Records (net direct +3)
 + Restaurant
 + Restaurant->name (char*)
 + Restaurant->pending_order (Queue*)
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- */
Restaurant* initialize_restaurant(char* name) {

	// Initialize restaurant
	Restaurant *new_restaurant;
	new_restaurant = (Restaurant *) malloc(sizeof(Restaurant));

	// Restaurant name
	new_restaurant->name = (char *) malloc(sizeof(char) * MAX_ITEM_NAME_LENGTH);
	strcpy(new_restaurant->name, name);

	// Restaurant menu
	new_restaurant->menu = load_menu(MENU_FNAME);

	// Restaurant num orders
	new_restaurant->num_completed_orders = 0;
	new_restaurant->num_pending_orders = 0;

	// Restaurant pending_order queue
	Queue *new_queue;
	new_queue = (Queue *) malloc(sizeof(Queue));
	new_queue->head = NULL;
	new_queue->tail = NULL;
	new_restaurant->pending_orders = new_queue;
}


/* --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
2nd TASK: Load Menu

Memory Allocation Records (net direct +4, with loop +2)
 + Menu
 + line (char*) once
 + line (char*) twice
 + Menu->item_codes (char**)
 +  			   -> code (for every idx) (char*) 
 + Menu->item_names (char**)
 +  			   -> name (for every idx) (char*) 
 + Menu->item_cost_per_unit (double*)

 - line (char*) once
 - line (char*) twice
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- */
Menu* load_menu(char* fname) {

	// Initialize menu
	Menu *new_menu;
	new_menu = (Menu *) malloc(sizeof(Menu));
	
	
	// Get number of items
	int count_items = 0;

	// Open file stream
	FILE *input_file;
	input_file = fopen(fname, "r");
	
	// Define whitespace characters
	const char whitespace[] = " \n\t\v\r";

	// Allocate a temp space for each line read from file
	char *line;
	line = (char *) malloc(ITEM_CODE_LENGTH + MAX_ITEM_NAME_LENGTH + MAX_PRECISION_DOUBLE);

	// Looping through each line from file
	// Incrementing count_item only if the line is not full of whitespace
	while (fgets(line, ITEM_CODE_LENGTH + MAX_ITEM_NAME_LENGTH + MAX_PRECISION_DOUBLE, input_file) != NULL) {
		if (strspn(line,whitespace) != strlen(line)) count_items++;
	}
	
	// Close file and free temp allocation
	fclose(input_file);
	free(line);

	// Initialize menu attributes
	char **item_codes_array;
	item_codes_array = (char **) malloc(sizeof(char*) * count_items);
	char **item_names_array;
	item_names_array = (char **) malloc(sizeof(char*) * count_items);
	double *item_cost_array = (double *) calloc(sizeof(double) , count_items);

	// Allocate a temp space for each line read from file again
	// Read file line by line
	line = (char *) malloc(ITEM_CODE_LENGTH + MAX_ITEM_NAME_LENGTH + MAX_PRECISION_DOUBLE);
	
	int idx = 0; // Temp index variable for populating array
	input_file = fopen(fname, "r"); // Open file stream again

	while (fgets(line, ITEM_CODE_LENGTH + MAX_ITEM_NAME_LENGTH + MAX_PRECISION_DOUBLE, input_file) != NULL) {
		
		// Split line by delimiter MENU_DELIM
		char *item_code = strtok(line, MENU_DELIM);
		char *item_name = strtok(NULL, MENU_DELIM);
		char *item_cost = strtok(NULL, MENU_DELIM);
		
		// Utilization of the helper function trimwhitespace()
		// in order to strip leading and trailing whitespace
		item_codes_array[idx] = (char *) calloc(sizeof(char) , ITEM_CODE_LENGTH);
		strcpy(item_codes_array[idx], trimwhitespace(item_code));
		
		item_names_array[idx] = (char *) calloc(sizeof(char) , MAX_ITEM_NAME_LENGTH);
		strcpy(item_names_array[idx], item_name);
		
		item_cost_array[idx] = strtod(item_cost+1, NULL); // +1 to skip dollar sign
		
		idx++;
		if (idx==count_items) break; // once all non-empty lines are processed, loop terminates
	}

	fclose(input_file);
	free(line);
	new_menu->num_items = count_items;
	new_menu->item_codes = item_codes_array;
	new_menu->item_names = item_names_array;
	new_menu->item_cost_per_unit = item_cost_array;

	return new_menu;
}


/* --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
3rd TASK: Build Order

Memory Allocation Records (net direct +3, with loop +1)
 + Order
 + Order->item_codes (char**)
 +                  -> code (for every idx) (char*) 
 + Order->item_quantities (int*)
 + cpy_quantities (char*)

 - cpy_quantities
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- */
Order* build_order(char* items, char* quantities) {

	// Initialize order
	Order *new_order;
	new_order = (Order *) malloc(sizeof(Order));

	// Get the number of items from string items
	int num_unique_item = strlen(items) / (ITEM_CODE_LENGTH - 1);
   
	// Array for item_codes
	char **item_codes_array_for_order;
	item_codes_array_for_order = (char **) malloc(sizeof(char*) * num_unique_item);

	// Array for item quantities
	int *item_q_array = (int *) malloc(sizeof(int) * num_unique_item);

	int code_idx = 0;

	// Allocate new variable and space for quantities
	// Otherwise the parameter is a read-only string literal that does not support manipulation
    char *cpy_quantities;
    cpy_quantities = (char *) calloc(sizeof(char), strlen(quantities)+1);
    strcpy(cpy_quantities,quantities);

	// Loop through each of the unique items
	for (int loop=0; loop<num_unique_item; loop++) {

		// Assign item code
		item_codes_array_for_order[loop] = (char *) calloc(sizeof(char) , (ITEM_CODE_LENGTH));
		strncpy(item_codes_array_for_order[loop], items, ITEM_CODE_LENGTH-1);
		items = items + ITEM_CODE_LENGTH-1;

        // Assign item quantities
		char *item_quantity;
		if (loop==0) item_quantity = strtok(cpy_quantities, MENU_DELIM);
		else item_quantity = strtok(NULL, MENU_DELIM);
        
		item_q_array[loop] = atoi(item_quantity);
	}

    free(cpy_quantities);
	new_order->num_items = num_unique_item;
	new_order->item_codes = item_codes_array_for_order;
	new_order->item_quantities = item_q_array;
	return new_order;
	
}


/* --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
4th TASK: Enqueue

--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- */
void enqueue_order(Order* order, Restaurant* restaurant) {

	Queue *alias_to_restaurant_queue;
	alias_to_restaurant_queue = restaurant->pending_orders;

	QueueNode *new_queueNode;
	new_queueNode = (QueueNode *) malloc(sizeof(QueueNode));
	new_queueNode->order = order;
    
    new_queueNode->next = NULL;

    if (alias_to_restaurant_queue->tail == NULL) {
        alias_to_restaurant_queue->tail = new_queueNode;
        alias_to_restaurant_queue->head = new_queueNode;
    }
    else {
	alias_to_restaurant_queue->tail->next = new_queueNode;
	alias_to_restaurant_queue->tail = new_queueNode;
    }

    restaurant->num_pending_orders = restaurant->num_pending_orders + 1;
}


/* --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
5th TASK: Dequeue

--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- */
Order* dequeue_order(Restaurant* restaurant) {
	Queue *alias_to_restaurant_queue;
	alias_to_restaurant_queue = restaurant->pending_orders;

	Order *output_order;
	output_order = (Order *) malloc(sizeof(Order));

	output_order = alias_to_restaurant_queue->head->order;
	QueueNode *temp_ptr_to_head;
	temp_ptr_to_head = alias_to_restaurant_queue->head;

    if (alias_to_restaurant_queue->head->next == NULL) {
		alias_to_restaurant_queue->head = NULL;
		alias_to_restaurant_queue->tail = NULL;
	}
    else alias_to_restaurant_queue->head = alias_to_restaurant_queue->head->next;

	free(temp_ptr_to_head);

    restaurant->num_completed_orders = restaurant->num_completed_orders + 1;
    restaurant->num_pending_orders = restaurant->num_pending_orders - 1;

	return output_order;
}



double get_item_cost(char* item_code, Menu* menu) {
	int i;
	for (i=0; i<(menu->num_items); i++){
		if (strcmp(item_code, (menu->item_codes)[i]) == 0) break;
	}
	return (menu->item_cost_per_unit)[i];
}


double get_order_subtotal(Order* order, Menu* menu) {
	double subtotal = 0;
	int i;
	for (i=0; i<(order->num_items); i++){
		char *this_item_code = (order->item_codes)[i];
		int this_item_quantity = (order->item_quantities)[i];
		subtotal += get_item_cost(this_item_code, menu) * this_item_quantity;
	}
	return subtotal;
}

double get_order_total(Order* order, Menu* menu) {
	return get_order_subtotal(order, menu) * ((double)TAX_RATE/100+1);
}


int get_num_completed_orders(Restaurant* restaurant) {
	return restaurant->num_completed_orders;
}

int get_num_pending_orders(Restaurant* restaurant) {
	return restaurant->num_pending_orders;
}

/* --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
CLEANING TASK: Clear Order

Memory Allocation Records (net direct -3, with loop -1)
 - Order
 - Order->item_codes (char**)
 -                  -> code (for every idx) (char*) 
 - Order->item_quantities (int*)

 ORDER IS ALL CLEARED
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- */
void clear_order(Order** order) {
	for (int i=0; i<(*order)->num_items; i++){
		free(((*order)->item_codes)[i]);
	}
	free((*order)->item_codes);
	free((*order)->item_quantities);
	free(*order);
	*order = NULL;
}


/* --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
CLEANING TASK: Clear Menu

Memory Allocation Records (net direct -4, with loop -2)
 - Menu
 - Menu->item_codes (char**)
 -  			   -> code (for every idx) (char*) 
 - Menu->item_names (char**)
 -  			   -> name (for every idx) (char*) 
 - Menu->item_cost_per_unit (double*)
 MENU IS ALL CLEARED
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- */
void clear_menu(Menu** menu) {

	for (int i=0; i<(*menu)->num_items; i++){
		free(((*menu)->item_codes)[i]);
		free(((*menu)->item_names)[i]);
	}

	free((*menu)->item_codes);
	free((*menu)->item_names);
	free((*menu)->item_cost_per_unit);
	free(*menu);
	*menu = NULL;
}


/* --------------------------------------------------------------------------------
--------------------------------------------------------------------------------
CLEANING TASK: Close Restaurant

Memory Allocation Records (net direct -3)
 - Restaurant
 - Restaurant->name (char*)
 - Restaurant->pending_order (Queue*)
 -                          -> head
 -                          -> tail
 - Order (every order)
		- Order->item_codes (char**)
		-                  -> code (for every idx) (char*) 
		- Order->item_quantities (int*)

 MENU IS ALL CLEARED
--------------------------------------------------------------------------------
-------------------------------------------------------------------------------- */
void close_restaurant(Restaurant** restaurant){

	clear_menu(&((*restaurant)->menu));

	QueueNode *temp = ((*restaurant)->pending_orders)->head;
	while ( temp != NULL){
		clear_order(&(temp->order));
		temp = temp->next;
	}
	if (((*restaurant)->pending_orders)->head != ((*restaurant)->pending_orders)->tail) free(((*restaurant)->pending_orders)->head);
	free(((*restaurant)->pending_orders)->tail);
	free((*restaurant)->pending_orders);
	
	free((*restaurant)->name);
	free(*restaurant);
	*restaurant = NULL;
}


void print_menu(Menu* menu){
	fprintf(stdout, "--- Menu ---\n");
	for (int i = 0; i < menu->num_items; i++){
		fprintf(stdout, "(%s) %s: %.2f\n", 
			menu->item_codes[i], 
			menu->item_names[i], 
			menu->item_cost_per_unit[i]	
		);
	}
}


void print_order(Order* order){
	for (int i = 0; i < order->num_items; i++){
		fprintf(
			stdout, 
			"%d x (%s)\n", 
			order->item_quantities[i], 
			order->item_codes[i]
		);
	}
}


void print_receipt(Order* order, Menu* menu){
	for (int i = 0; i < order->num_items; i++){
		double item_cost = get_item_cost(order->item_codes[i], menu);
		fprintf(
			stdout, 
			"%d x (%s)\n @$%.2f ea \t %.2f\n", 
			order->item_quantities[i],
			order->item_codes[i], 
			item_cost,
			item_cost * order->item_quantities[i]
		);
	}
	double order_subtotal = get_order_subtotal(order, menu);
	double order_total = get_order_total(order, menu);
	
	fprintf(stdout, "Subtotal: \t %.2f\n", order_subtotal);
	fprintf(stdout, "               -------\n");
	fprintf(stdout, "Tax %d%%: \t$%.2f\n", TAX_RATE, order_total);
	fprintf(stdout, "              ========\n");
}



/*

int main() {
	Menu *menu = load_menu("menu.txt");
	print_menu(menu);
}

*/