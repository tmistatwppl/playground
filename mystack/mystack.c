#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);

int parse_int(char*);

static unsigned DBGLEVEL = 0;

#define MSG_LEVEL1 1
#define dbgprintf1(fmt, args...) if(MSG_LEVEL1 & DBGLEVEL) printf(fmt, ## args)
#define MSG_LEVEL7 (1<<2)
#define dbgprintf7(fmt, args...) if(MSG_LEVEL7 & DBGLEVEL) printf(fmt, ## args)



 typedef struct mystack{    
    unsigned int maxS; 
    unsigned int top;
    unsigned int* st;    
 }mystack_t;
 

void mystack_init(mystack_t* ptrS, unsigned int maxSize) {
    ptrS->st = (mystack_t*)malloc(maxSize*sizeof(mystack_t*));
    ptrS->top = 0;
    ptrS->maxS = maxSize;
}

void mystack_pop(mystack_t* ptrS)
{    
    if(ptrS->top == 0){
        dbgprintf1("mystack_pop empty pop is nop\n");
    }
    else {
        ptrS->top--;
    }
}
 
void mystack_push(mystack_t* ptrS, unsigned int val)  
{
    if(ptrS->st == NULL ) {dbgprintf1("mystack_push stack not init\n");return;}
    ptrS->st[ptrS->top]=val;
    ptrS->top++;
}

unsigned int mystack_print(mystack_t* ptrS){
    unsigned int max = 0;
    for(unsigned int n=0;n < ptrS->top;n++){
        dbgprintf7("%u\n",ptrS->st[ptrS->top-1-n]);
        max = (max > ptrS->st[ptrS->top-1-n]) ? max : ptrS->st[ptrS->top-1-n];
    }
    return max;
}



int* getMax(int operations_count, char** operations, int* result_count) {
    *result_count = 0;
    int* maxV = malloc(operations_count*sizeof(int));
    
    int v = 0, vISvalid = 0;
    mystack_t S;
    mystack_init(&S,operations_count);
    //is ULONG_MAX okey for x 10^9 
    //dbgprintf7("ULONG_MAX is okey ?  %d",(ULONG_MAX > 1000000000));
    
    for(int n = 0; n < operations_count; n++){
        char query_type = operations[n][0];
                                
        if(query_type == '1') {//push
            char* endptr;
            unsigned int query_val = strtol(&operations[n][2], &endptr, 10);
            dbgprintf1("query %d type %c val %u\n ", n, query_type, query_val);
            mystack_push(&S,query_val);
            
            dbgprintf7(" v from mystack_print is %d\n", mystack_print(&S));    
            vISvalid = 0;        
        }
        else if(query_type == '2') {//pop
            dbgprintf1("query %d type %c \n", n, query_type);
            mystack_pop(&S);
            
            dbgprintf7(" v from mystack_print is %d\n", mystack_print(&S));    
            vISvalid = 0;
        }
        else if(query_type == '3') { //print
            dbgprintf1("query %d type %c \n", n, query_type);  
            v = mystack_print(&S);
            dbgprintf7(" v from mystack_print is to be saved %d\n", v);
            //register new result and save val
            (*result_count)++;
            maxV[(*(result_count)-1)] = v;
            
        }
        else printf("WARN: unsupported query type\n");
    }
    return maxV;
}

int main()
{
    //FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
    FILE* inputTC = fopen("./tc/tc17.in","r");


    int n = parse_int(ltrim(rtrim(readline(inputTC))));

    char** ops = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* ops_item = readline(inputTC);

        *(ops + i) = ops_item;
    }

    int res_count;
    int* res = getMax(n, ops, &res_count);

    for (int i = 0; i < res_count; i++) {
        printf( "%d", *(res + i));

        if (i != res_count - 1) {
            printf( "\n");
        }
    }

    printf( "\n");

    fclose(inputTC);

    return 0;
}

//char* readline() {
char* readline(FILE* inputTC) {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        //char* line = fgets(cursor, alloc_length - data_length, stdin);
        char* line = fgets(cursor, alloc_length - data_length, inputTC);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
