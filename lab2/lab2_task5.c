#include <stdio.h>
#include <stdlib.h>




int main(){

	int *p = (int *) malloc ( sizeof (int));
	int *q = (int *) malloc ( sizeof (int) * 2);
	*p = 1;
	*q = 10;
	printf("Before\n");
	printf("%d and %p\n",*p,p);
	printf("%d and %p\n",*(p+1),(p+1));
	printf("%d and %p\n",*q,q);
	printf("%d and %p\n",*(q+1),(q+1));
	q = p;

	printf("After\n");
	printf("%d and %p\n",*p,p);
	printf("%d and %p\n",*(p+1),(p+1));
	printf("%d and %p\n",*q,q);
	printf("%d and %p\n",*(q+1),(q+1));

	free (q);
	free (p);
	
}