#include <stdio.h>
#include <stdlib.h>




int main(){
	int x = 0;
	int* ptr_x = &x;
	int y = *ptr_x;
	fprintf(stdout, "x contains the value %d at location %p\n", x, &x);
	fprintf(stdout, "y contains the value %d at location %p\n", y, &y);
	
}