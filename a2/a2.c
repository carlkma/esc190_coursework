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
        new_string[i] = num % 2;
        num /= 2;
    }

    return new_string;
}

char xor_decrypt(char *s){
    int num = 0;
    for (int i=6; i>=0; i--){
        num += s[i] * pow(2, 6-i);
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
    new_code = (char*) calloc(sizeof(char) , 257);

    for (int i=0; i<256; i++){
        //new_code[i] = 0;
        if (is_forbidden(i)) new_code[i] = 1;
        if (is_forbidden_but_white(i)) new_code[i] = 0;
    }

    new_code[256] = '\0';

    while (*msg != '\0'){
        char the_char = *msg;
        char *the_char_in_bits = xor_encrypt(the_char);
        
        
        insert_bits_to_code(the_char_in_bits, &new_code, &current_index);
        msg++;
    }
    insert_bits_to_code(xor_encrypt('\0'),&new_code, &current_index);
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
        
        message[msg_index] = c;
        if (c=='\0') {
            free(new_string);
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
        hex += code[i] * 8;
        hex += code[i+1] * 4;
        hex += code[i+2] * 2;
        hex += code[i+3] * 1;

        printf("now index is %d, and %d \n", i, hex);

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
        dec = hex;
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


int calc_ld(char *sandy, char *cima){
    //add code here
}




int main(){

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
 
    return 0;
}