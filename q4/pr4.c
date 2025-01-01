#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();

void printf_num(int n){
    if ( n == 1 ) printf("one");
    else if ( n == 2 ) printf("two");
    else if ( n == 3 ) printf("three");
    else if ( n == 4 ) printf("four");
    else if ( n == 5 ) printf("five");
    else if ( n == 6 ) printf("six");
    else if ( n == 7 ) printf("seven");
    else if ( n == 8 ) printf("eight");
    else if ( n == 9 ) printf("nine");
    else if ( n  > 9 ) printf("Greater than 9");
    else 
       printf("condition not handeled");
    printf("\n");
};

void printf_num_opt(int n) {

    //s is not needed only for checking consts
    char string[30]; sprintf(string,"Greater than 9 from string[30]");

    //char* const s_to_print=NULL; //wrong s_to_print later modified not the lvalue
    const char* s_to_print=NULL;

    //char* const arr[] = {
    const char* arr[] = {
    //const char* const arr[] = {    
    
        "condition not handeled", // arr[0]
        "one","two","three","four","five","six","seven","eight","nine", // arr[1]..arr[9]
        //"Greater than 9"        // arr[10]    
        string                    // arr[10]
    };
    // need char* const arr for that not to work
    arr[1] = "modificaiton should not be allowed"; 
    //arr[2] = string;
    //arr[10][1] = 'x';
    

    if( n > 0 && n < 10 ) 
        s_to_print = arr[n];
    else if ( n > 10 && n < 100000 ) 
        s_to_print = arr[10];
    else 
        s_to_print = arr[0];
    
    //printf
    printf("%s\n",s_to_print);

}

typedef void(*fptr)(int);



int main()
{
    char* n_endptr;
    char* n_str = readline();
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    fptr ftable[2] = {printf_num,printf_num_opt};

    //printf_num(n);
    //printf_num_opt(n);
    ftable[0](n);
    ftable[1](n);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length*sizeof(*data));

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) { break; }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') { break; }

        size_t new_length = alloc_length << 1;
        data = realloc(data, new_length);

        if (!data) { break; }

        alloc_length = new_length;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
    }

    data = realloc(data, data_length);

    return data;
}

