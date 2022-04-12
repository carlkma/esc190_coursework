/*----------------------------------------
ESC190 Assignment 2
Apr 11, 2022
Ma, Carl Ka To
Xu, Shenxiaozhu
V1.6 test submission, to be updated
----------------------------------------*/

#include "a2.h"
#define MAX_MSG_LENGTH 37 // 256/7=36.6

// helper

void print_bit(char *new_string){
    for (int i=0; i<7; i++){
        printf("%i", new_string[i]);
    }
    printf("\n");
}

void print_code(char *new_code){
    for (int i=0; i<256; i++){
        printf("%i", new_code[i]);
        if ((i+1)%16==0) printf("\n");
    }
    return;
}

int bitwise_xor(int value){
    return value ^ KEY;
}

char *xor_encrypt(char c){
    int num = bitwise_xor(c);
    char *new_string;
    new_string = (char*) malloc(sizeof(char) * 8);
    new_string[7] = '\0';

    for (int i=6; i>=0; i--){
        new_string[i] = (num % 2) + '0';
        num /= 2;
    }
    return new_string;
}

char xor_decrypt(char *s){

    int num = 0;
    for (int i=6; i>=0; i--){
        num += (s[i]-48) * pow(2, 6-i);
    }
    
    return bitwise_xor(num);
}

int is_forbidden(int index){

    // top-left block
    if (index>=0 && index<=4) return 1;
    if (index>=16 && index<=20) return 1;
    if (index>=32 && index<=36) return 1;
    if (index>=48 && index<=52) return 1;
    if (index>=64 && index<=68) return 1;

    // top-right block
    if (index>=11 && index<=15) return 1;
    if (index>=27 && index<=31) return 1;
    if (index>=43 && index<=47) return 1;
    if (index>=59 && index<=63) return 1;
    if (index>=75 && index<=79) return 1;

    // bottom-left block
    if (index>=176 && index<=180) return 1;
    if (index>=192 && index<=196) return 1;
    if (index>=208 && index<=212) return 1;
    if (index>=224 && index<=228) return 1;
    if (index>=240 && index<=244) return 1;

    // bottom-right
    if (index==255) return 1;
    
    // otherwise, not forbidden
    return 0;

}

int is_forbidden_but_white(int index){
    if (index>=17 && index<=19) return 1;
    if (index>=49 && index<=51) return 1;

    if (index>=28 && index<=30) return 1;
    if (index>=60 && index<=62) return 1;

    if (index>=193 && index<=195) return 1;
    if (index>=225 && index<=227) return 1;

    if (index==33 || index==35) return 1;
    if (index==44 || index==46) return 1;
    if (index==209 || index==211) return 1;

    return 0;
}

int increment(int current_index){
    if (current_index >= 255) return -1;
    
    do current_index++;
    while (is_forbidden(current_index));
    
    return current_index;
}

void insert_bits_to_code(char* bits, char** ptr_code, int* current_index){
    for (int i=0; i<7; i++){
        (*ptr_code)[*current_index] = bits[i];
        (*current_index) = increment(*current_index);
    }
}

char *gen_code(char *msg){
    char *new_code;
    int current_index = 5;
    new_code = (char*) malloc(sizeof(char) * 257);

    for (int i=0; i<256; i++){
        new_code[i] = '0';
        if (is_forbidden(i)) new_code[i] = '1';
        if (is_forbidden_but_white(i)) new_code[i] = '0';
    }

    new_code[256] = '\0';

    while (*msg != '\0'){
        char the_char = *msg;
        char *the_char_in_bits = xor_encrypt(the_char);
        
        
        insert_bits_to_code(the_char_in_bits, &new_code, &current_index);
        free(the_char_in_bits);
        msg++;
    }
    char *ending = xor_encrypt('\0');
    insert_bits_to_code(ending,&new_code, &current_index);
    free(ending);
    return new_code;


}

char *read_code(char *code){
    int current_index = 5;
    char *message;
    message = (char*) malloc(sizeof(char) * MAX_MSG_LENGTH);

    for (int msg_index=0; msg_index<MAX_MSG_LENGTH; msg_index++) {
        char *new_string;
        new_string = (char*) malloc(sizeof(char) * 8);

        for (int i=0; i<7; i++){
            new_string[i] = code[current_index];
            current_index = increment(current_index);
        }
        char c = xor_decrypt(new_string);
        free(new_string);
        
        message[msg_index] = c;
        if (c=='\0') {
            break;
        }

    }
    return message;
}

char *compress(char *code){
    char *hex_output;
    hex_output = (char*) malloc(sizeof(char) * 65);
    for (int i=0; i<=252; i+=4){
        int hex = 0;
        hex += (code[i] - '0') * 8;
        hex += (code[i+1] - '0') * 4;
        hex += (code[i+2] - '0') * 2;
        hex += (code[i+3] - '0') * 1;

        if (hex<10){
            hex_output[i/4] = hex + '0';
            continue;
        }
        if (hex==10) hex_output[i/4] = 'A';
        if (hex==11) hex_output[i/4] = 'B';
        if (hex==12) hex_output[i/4] = 'C';
        if (hex==13) hex_output[i/4] = 'D';
        if (hex==14) hex_output[i/4] = 'E';
        if (hex==15) hex_output[i/4] = 'F';
    }
    
    hex_output[64] = '\0';
    return hex_output;
}

char *decompress(char *code){
    char *bin_output;
    bin_output = (char*) malloc(sizeof(char) * 257);
    for (int i=0; i<=63; i++){
        
        
        char hex = code[i];
        int dec = 0;
        dec = hex - 48;
        if (hex=='A') dec=10;
        if (hex=='B') dec=11;
        if (hex=='C') dec=12;
        if (hex=='D') dec=13;
        if (hex=='E') dec=14;
        if (hex=='F') dec=15;

        
        for (int j=3; j>=0; j--){
        bin_output[i*4+j] = (dec % 2) + '0';
        dec /= 2;
    }
    }
    bin_output[256] = '\0';
    return bin_output;

}

int get_length(char *arr){
    int length = 0;
    if (arr==NULL) return length;
    while (*arr != '\0'){
        length++;
        arr++;
    }
    return length;
}

int get_min(int a1, int b1, int c1){
    int temp_min = 2147483647;
    if (a1<temp_min) temp_min=a1;
    if (b1<temp_min) temp_min=b1;
    if (c1<temp_min) temp_min=c1;
    return temp_min;
}

int calc_ld(char *sandy, char *cima){

    if (sandy==NULL || *sandy=='\0') return get_length(cima);
    if (cima==NULL || *cima=='\0') return get_length(sandy);

    if (sandy[0]==cima[0]){
        char *new_sandy0, *new_cima0;
        new_sandy0 = sandy+1;
        new_cima0 = cima+1;
        return calc_ld(new_sandy0, new_cima0);
    }
    else{
        
        char *new_sandy1, *new_cima1;
        char *new_sandy2, *new_cima2;
        char *new_sandy3, *new_cima3;

        new_sandy1 = sandy+1;
        new_cima1 = cima;

        new_sandy2 = sandy;
        new_cima2 = cima+1;

        new_sandy3 = sandy+1;
        new_cima3 = cima+1;

        // case 1
        int case1 = calc_ld(new_cima1, new_sandy1);
        // case 2
        int case2 = calc_ld(new_cima2, new_sandy2);
        // case 3
        int case3 = calc_ld(new_cima3, new_sandy3);

        return 1+get_min(case1,case2,case3);
    }
}

/*
int main(){

    char *task1 = xor_encrypt('A');
    //printf("%s", task1);

    int asdf = 499/10;
    printf("%d", asdf);
    exit(1);



    char c = xor_decrypt(task1);
    //printf("Decrypted is %c", c);
    exit(1);
    
    char *test_string = "Program in C!";
    char *result = gen_code(test_string);
    print_code(result);
    printf("%s", read_code(result));
    char * result2 = compress(result);

    printf("%s", result2);
    printf("done\n");

    char *result3 = decompress(result2);
    printf("%s", result3);
    printf("\n");
    printf("%s", result);
    

   // Task 4 test
   int answer = calc_ld("COMMENCE","CODING");
   answer = calc_ld("COMMENCE","PROCRASTINATING");
   printf("%d", answer);
   
   
   return 0;
}
*/