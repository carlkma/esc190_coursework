//#include <stdio.h>
#include <stdlib.h>


void append(int** x, int* n, int val){

	*n = *n + 1;
	//printf("%d\n", *n);
	*x = (int*) realloc(*x, sizeof(int) * (*n));

	
	(*x)[*n - 1] = val;

	//printf("%d\n", *(*x+0));
	//printf("%d\n", *(*x+1));
	//printf("%d\n", *(*x+2));

	

}
/*
int main(){
	int size = 2;
	int* n = &size;
	int* arr = (int*) malloc( sizeof (int) * *n);
	

	*arr = 1;
	*(arr+1) = 2;
	//*(arr+2) = 3;

	int** x = &arr;
	append(x,n,50);

	int* the_new_arr = *x;

	printf("%d and %d and %d and %d and %d", *(the_new_arr), *(the_new_arr+1),*(the_new_arr+2),*(the_new_arr+3),*(the_new_arr+4));
}
*/
