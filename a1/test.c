#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MENU_DELIM ","
#define ITEM_CODE_LENGTH 3
#define MAX_ITEM_NAME_LENGTH 100
#define MAX_ITEM_QUANTITY_DIGITS 20
#define MAX_PRECISION_DOUBLE 16

#define MENU_DELIM ","
#define MENU_FNAME "menu.txt"

#define TAX_RATE 13
typedef struct Menu {
	int num_items;
	char** item_codes; 
	char** item_names; 
	double* item_cost_per_unit;
} Menu;


Menu* load_menu(char* fname) {

	// Initialize menu
	Menu *new_menu;
	new_menu = (Menu *) malloc(sizeof(Menu));

	// Initialize menu attributes
	int count_items = 1;
	
	// Get number of items
	FILE *input_file;
	input_file = fopen(fname, "r");
	char c;
	for (c=getc(input_file); c!=EOF; c=getc(input_file)){
		if (c=='\n') count_items++;
	}
	fclose(input_file);

	// Continue to initialize menu attributes
	char **item_codes_array;
	item_codes_array = (char **) malloc(sizeof(char*) * count_items);
	char **item_names_array;
	item_names_array = (char **) malloc(sizeof(char*) * count_items);
	double *item_cost_array = (double *) malloc(sizeof(double) * count_items);

	// Read file line by line
	char *line;
	line = (char *) malloc(ITEM_CODE_LENGTH + MAX_ITEM_NAME_LENGTH + MAX_PRECISION_DOUBLE);

	int idx = 0;
	input_file = fopen(fname, "r");
	while (fgets(line, ITEM_CODE_LENGTH + MAX_ITEM_NAME_LENGTH + MAX_PRECISION_DOUBLE, input_file) != NULL) {
		

		char *item_code = strtok(line, MENU_DELIM);
		char *item_name = strtok(NULL, MENU_DELIM);
		char *item_cost = strtok(NULL, MENU_DELIM);

		item_codes_array[idx] = (char *) malloc(sizeof(char) * ITEM_CODE_LENGTH);
		strcpy(item_codes_array[idx], item_code);

		item_names_array[idx] = (char *) malloc(sizeof(char) * MAX_ITEM_NAME_LENGTH);
		strcpy(item_names_array[idx], item_name);

		//char *item_cost_clean = item_cost+1; // removes first character
  		//item_cost_clean[strlen(item_cost)-1] = '\0'; // removes last character
		item_cost_array[idx] = strtod(item_cost+1, NULL);
		idx++;
	}

	fclose(input_file);
	new_menu->num_items = count_items;
	new_menu->item_codes = item_codes_array;
	new_menu->item_names = item_names_array;
	new_menu->item_cost_per_unit = item_cost_array;

	return new_menu;

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


int main() {
	Menu *new;
	new = load_menu("menu.txt"); 
	print_menu(new);
}