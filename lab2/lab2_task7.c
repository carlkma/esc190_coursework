#include <stdio.h>
#include <stdlib.h>




int main(){
	int x = 0;
	int* ptr = &x;
	fprintf(stdout, "x contains the value %d at location %p\n", *ptr, ptr);
	
}