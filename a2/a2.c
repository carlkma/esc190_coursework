#include "a2.h"

int bitwise_xor(int value){
    return value ^ KEY;
}

char *xor_encrypt(char c){
    int num = bitwise_xor(c);
    printf("%i", num);

    char *new_string;
    new_string = (char*) malloc(sizeof(char) * 8);
    for (int i=6; i>0; i--){
        new_string[i] = num % 2;
        num /= 2;
    }
    return new_string;
}

char xor_decrypt(char *s){
    //add code here
}

char *gen_code(char *msg){
    //add code here;
}

char *read_code(char *code){
    //add code here
}

char *compress(char *code){
    //add code here
}

char *decompress(char *code){
    //add code here
}

int calc_ld(char *sandy, char *cima){
    //add code here
}


// helper
void print_array(char *arr){
    while (arr!=NULL){
        printf("%i",arr);
        arr++;
    }
    printf("\n");
    
}

int main(){
    int answer;
    answer = bitwise_xor(84);
    printf("%i", answer);
    return 0;
}