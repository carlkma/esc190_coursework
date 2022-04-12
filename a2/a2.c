/*----------------------------------------
ESC190 Assignment 2
Apr 11, 2022
Ma, Carl Ka To
Xu, Shenxiaozhu
V1.7 Final submission, unless updated

Program Summary
 1.1 "Encrypt" a <ascii character> by performing bitwise XOR with <a KEY character>
 1.2 "Decrypt" the encrypted character
 2.1 Generate a QR-code-like "SC code", storing <message>, a list of encrypted characters
 2.2 Read the SC code
 3.1 "Compress" a bit-string into a hex-string
 3.2 "Decompress" the hex-string back to a bit-string
 4.0 Calcuate the Levenshtein distance between two strings, in order to assess data corruption

Concepts
 - bit manipulation
   - XOR operator
   - bin, hex, decimal conversion
 - dynamic memory allocation
----------------------------------------*/
#include "a2.h"
#define MAX_MSG_LENGTH 37
// The SC code contains 16*16=256 bits
// Every char takes 7 bits
// Ignoring the template (fixed) bits of the SC code,
// the max char count of a message is approx:
// 256/7 = 36.6 = 37

// perform bitwise XOR between <int value> and <char KEY>
// input: <int value>, an ASCII value in decimal
// output: <int output>, an ASCII value in decimal
int bitwise_xor(int value){
    return value ^ KEY;
}

// -------------------- SECTION 1 --------------------

// 1.1 Encrypt
// input: <char c>, equivalent to an ASCII value in decimal
// output: <char *output>, a bit-string
char *xor_encrypt(char c){
    int num = bitwise_xor(c);
    // num represents the encrypted <char c>, in the form of an ASCII value in decimal

    // dynamically allocate space for a bit-string
    // 7-bit + 1-bit for null terminator
    char *new_string;
    new_string = (char*) malloc(sizeof(char) * 8);
    new_string[7] = '\0';

    // convert ASCII value from decimal to binary
    // using the mod method 
    for (int i=6; i>=0; i--){
        new_string[i] = (num % 2) + '0';
        // + '0' is required to store the character 0 or 1 (ASCII 48, ASCII 49)
        // as opposed to the integer 0 or 1 
        num /= 2;
    }
    return new_string;
}

// 1.2 Decrypt
// input: <char *s>, a bit-string of 7 char-digits
// output: <char output>, an ASCII value in decimal
char xor_decrypt(char *s){

    // convert ASCII value from binary to decimal
    int num = 0;
    for (int i=6; i>=0; i--){
        num += (s[i]-'0') * pow(2, 6-i);
        // - '0' is required to change the character 0 or 1 (ASCII 48, ASCII 49)
        // back to the math-compatible integer 0 or 1
    }
    // num is in decimal,
    // but it still represents the encrypted <char c>
    // bitwise XOR is performed again to decrypt (revert the encryption)
    return bitwise_xor(num);
}

// -------------------- SECTION 2 --------------------

// before implementing the SC code, template features are defined:
// - a 256 char-digit string containing char '0' (ASCII 48) and '1' (ASCII 49)
// - visualized as a 16x16 grid
// - indexing from 0 to 255
// - all four corners contain fixed, template bits
// - similar to the mandated position and alignment patterns in a QR code

// determine if position in the SC code is a template position
// i.e. forbidden to modify
// input: <int index>, any index in [0, 255]
// output: <int output>, 0 for FREE TO USE, 1 for FORBIDDEN
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

// as per SC code definitions, some template bits are colored black,
// while some template bits are colored white

// differentiate the similarly-forbidden yet differently-colored positions
// input: <int index>, any index in [0, 255]
// output: <int output>, 0 for all other blocks, 1 for FORBIDDEN AND WHITE
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
// note that, to check if a position is forbidden, use is_forbidden()
// the above is_forbidden_but_white() function is used only once
// to change the color of certain forbidden positions

// determine the next free (not forbidden) index
// input: <int current_index>, any index in [0, 255]
// output: <int next_index>, the next free index
int increment(int current_index){

    // edge condition
    if (current_index >= 255) return -1;
    
    // increment current_index once
    do current_index++;
    while (is_forbidden(current_index));
    // loop if the incremented current_index is forbidden
    
    return current_index;
}

// insert a bit-string into the SC code at position current_index
// primary input: <char *s>, a bit-string of 7 char-digits
// input 2: double pointer to SC code (so that the char string can be modified)
// input 3: pointer to current_index (so that the current_index can be incremented)
// output: VOID, but both *ptr_code and *current_index modified
void insert_bits_to_code(char* bits, char** ptr_code, int* current_index){

    // loop thru each of the 7 char-digits
    for (int i=0; i<7; i++){
        (*ptr_code)[*current_index] = bits[i];
        (*current_index) = increment(*current_index);
    }
}

// generate SC code from <char* msg>
// input: <char *msg>, a string to be encrypted
// output: <char* output>, the SC code corresponding encrypted string
char *gen_code(char *msg){

    char *new_code;
    int current_index = 5; // the first free index is 5
    new_code = (char*) malloc(sizeof(char) * 257);
    // 256-bit + 1-bit for 256 char-digits and null terminator

    // loop once to initialize SC code template
    for (int i=0; i<256; i++){
        new_code[i] = '0';
        if (is_forbidden(i)) new_code[i] = '1';
        if (is_forbidden_but_white(i)) new_code[i] = '0';
    }

    new_code[256] = '\0'; // null terminator

    // loop thru each char in msg (until null terminator)
    while (*msg != '\0'){
        char the_char = *msg; // retrieve a char from the char string
        char *the_char_in_bits = xor_encrypt(the_char); // get bit-string of the encrypted char
        
        insert_bits_to_code(the_char_in_bits, &new_code, &current_index);
        // the temp bit-string storing the encrypted char is now useless
        free(the_char_in_bits);
        msg++;
    }

    // add an encrypted version of the null terminator to the SC code
    char *ending = xor_encrypt('\0');
    insert_bits_to_code(ending,&new_code, &current_index);
    // the temp bit-string storing the encrypted null terminator is now useless
    free(ending);
    return new_code;
}

// read a SC code
// input: <char *code>, a SC code
// output: <char *output>, the string decrypted from the SC code
char *read_code(char *code){

    // Again, the first free index is 5
    int current_index = 5;

    // dynamically allocate space for a message string
    char *message;
    message = (char*) malloc(sizeof(char) * MAX_MSG_LENGTH); // see previously defined MAX_MSG_LENGTH

    // loop thru index of newly allocated message string
    for (int msg_index=0; msg_index<MAX_MSG_LENGTH; msg_index++) {
        
        // dynamically allocate space for a bit-string, with values read from the SC code
        char *new_string;
        new_string = (char*) malloc(sizeof(char) * 8); // 7-bit + 1-bit

        // get 7 consecutive (free) char-digits from the SC code
        for (int i=0; i<7; i++){
            new_string[i] = code[current_index];
            current_index = increment(current_index);
        }

        // decrypt the bit-string
        char c = xor_decrypt(new_string);
        // the temp bit-string storing the encrypted character is now useless
        free(new_string);
        
        // store the decrypted character in the message string
        message[msg_index] = c;

        // if the decrypted char happens to be the null terminator, break the loop
        if (c=='\0') break;
    }
    return message;
}

// -------------------- SECTION 3 --------------------

// convert binary to hex
// input: <char *code>, a 256 char-digit string of '0' (ASCII 48) and '1' (ASCII 49)
// output: <char *output>, a 64 char-digit string of '0-9, A-F'
char *compress(char *code){

    // dynamically allocate space for a hex-string
    char *hex_output;
    hex_output = (char*) malloc(sizeof(char) * 65);
    // 64-bit + 1-bit for null terminator

    // loop through every four bit in bit-string
    for (int i=0; i<=252; i+=4){
        
        // temp number, in decimal, in [0, 15]
        int hex = 0;

        // - '0' is required to change the character 0 or 1 (ASCII 48, ASCII 49)
        // back to the math-compatible integer 0 or 1
        hex += (code[i] - '0') * 8;
        hex += (code[i+1] - '0') * 4;
        hex += (code[i+2] - '0') * 2;
        hex += (code[i+3] - '0') * 1;

        if (hex<10){
            // note the index i/4 in hex_output, as every four indices
            // in the bit-string correspond to every one index in the hex-string
            hex_output[i/4] = hex + '0';
            // + '0' is required to store the character 0 or 1 (ASCII 48, ASCII 49)
            // as opposed to the integer 0 or 1 
            continue;
        }
        // convert 10-15 to 'A'-'F'
        if (hex==10) hex_output[i/4] = 'A';
        if (hex==11) hex_output[i/4] = 'B';
        if (hex==12) hex_output[i/4] = 'C';
        if (hex==13) hex_output[i/4] = 'D';
        if (hex==14) hex_output[i/4] = 'E';
        if (hex==15) hex_output[i/4] = 'F';
    }
    
    hex_output[64] = '\0'; // null terminator
    return hex_output;
}

// convert hex to binary
// input: <char *code>, a 64 char-digit string of '0-9, A-F'
// output: <char *output>, a 256 char-digit string of '0' (ASCII 48) and '1' (ASCII 49)
char *decompress(char *code){
    
    // dynamically allocate space for a hex-string
    char *bin_output;
    bin_output = (char*) malloc(sizeof(char) * 257);
    // 256-bit + 1-bit for null terminator

    // loop through every bit in hex-string
    for (int i=0; i<=63; i++){
        
        char hex = code[i]; // get the hex-character
        int dec = 0; // initialize a temp decimal number
        
        // if hex is '0'-'9', the following yields a valid decimal number corresponding to the hex-character
        // i.e. '5'-'0' = ASCII 53 - ASCII 48 = 5 (an int)
        dec = hex - '0';

        // however, dec would be an incorrect int if hex is 'A'-'F'
        // i.e. 'A'-'0' = ASCII 65 - ASCIII 48 = 17 (an int, but incorrect)
        // hence the following
        if (hex=='A') dec=10;
        if (hex=='B') dec=11;
        if (hex=='C') dec=12;
        if (hex=='D') dec=13;
        if (hex=='E') dec=14;
        if (hex=='F') dec=15;

        // quick conversion from decimal to binary (again)
        for (int j=3; j>=0; j--){
            // note the index i*4+j

            // i*4 because every one index in the hex-string
            // correspond to four indices in the bit-string

            // +j because the mod method returns the rightmost-bit 
            bin_output[i*4+j] = (dec % 2) + '0';
            // + '0' is required to store the character 0 or 1 (ASCII 48, ASCII 49)
            dec /= 2;
        }
    }
    bin_output[256] = '\0'; // null terminator
    return bin_output;
}

// -------------------- SECTION 4 --------------------

// get length of a char string
// input: <char *arr>, a char string of unknown length
// output: <int output>, length of the char string
int get_length(char *arr){
    int length = 0;
    if (arr==NULL) return length; // edge case
    while (*arr != '\0'){
        length++; // increment length
        arr++; // increment pointer
    }
    return length;
}

// get min of three int
// input: <int a1>, <int b1>, <int c1>
// output: value of the min of the input int
int get_min(int a1, int b1, int c1){
    int temp_min = 2147483647; // MAX value of int type
    if (a1<temp_min) temp_min=a1;
    if (b1<temp_min) temp_min=b1;
    if (c1<temp_min) temp_min=c1;
    return temp_min;
}

// recursive approach to calculate Levenshtein distance (LD)
// input: <char *sandy>, <char *cim>, two strings
// output: <int LD>, the calculated LD between the two strings
int calc_ld(char *sandy, char *cima){

    // LD is length of cima if sandy is an empty string
    if (sandy==NULL || *sandy=='\0') return get_length(cima);

    // LD is length of sandy if cima is an empty string
    if (cima==NULL || *cima=='\0') return get_length(sandy);

    // if the first characters of each string are the same, then...
    if (sandy[0]==cima[0]){
        char *new_sandy0, *new_cima0;
        new_sandy0 = sandy+1;
        new_cima0 = cima+1;
        // LD is equal to the LD between sandy and cima,
        // ignoring their first characters 
        return calc_ld(new_sandy0, new_cima0);
    }
    // if first characters are not the same, then:
    // LD is 1 + the min of
    //           - LD between original C and S without first char
    //           - LD between C without first char and original S
    //           - LD between C without first char and S without first char
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

// ------------------------------ END OF ESC190 Assignment 2 ------------------------------