#include <stdio.h>
#include <stdlib.h>




int main(){


	int *p = (int *) malloc(sizeof(int));
	fprintf (stdout , "Value stored at address %p: %d\n", p, *p);
	*p = 1;
	fprintf (stdout , "Value stored at address %p: %d\n", p, *p);
	free(p);
	fprintf (stdout , "Value stored at address %p: %d\n", p, *p);


}