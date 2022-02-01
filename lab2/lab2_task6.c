#include <stdio.h>
#include <stdlib.h>


int main(){
	

	int** p;
	int x = 10;
	int y = 11;
	int z = 12;
	int* ptr = (int*) malloc( sizeof (int) * 3);
	*ptr = x;
	*(ptr+1) = y;
	*(ptr+2) = z;

	/*
	printf("xyz\n");
	printf("%d and %p\n",x,&x);
	printf("%d and %p\n",y,&y);
	printf("%d and %p\n",z,&z);

	printf("Before\n");
	printf("%d and %p\n",*(ptr),ptr);
	printf("%d and %p\n",*(ptr+1),(ptr+1));
	printf("%d and %p\n",*(ptr+2),(ptr+2));
	*/

	p = &ptr;

	/*
	printf("After\n");
	printf("%d and %p and %p\n",**p, *p,p);
	printf("%d and %p and %p\n",*(*(p)+1), *(p)+1,(p));
	printf("Done!\n");
	*/

	free(ptr);
	// free(p);

}