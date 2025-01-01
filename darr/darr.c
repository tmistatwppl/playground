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

#define myPRINTFDBG
#ifdef myPRINTFDBG
#define printfdbg(fmt, args...)    printf(fmt, ## args)
#else
#define printfdbg(fmt, args...)
#endif

/*
 * Complete the 'dynamicArray' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. 2D_INTEGER_ARRAY queries
 */

/*
 * To return the integer array from the function, you should:
 *     - Store the size of the array to be returned in the result_count variable
 *     - Allocate the array statically or dynamically
 *
 * For example,
 * int* return_integer_array_using_static_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     static int a[5] = {1, 2, 3, 4, 5};
 *
 *     return a;
 * }
 *
 * int* return_integer_array_using_dynamic_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     int *a = malloc(5 * sizeof(int));
 *
 *     for (int i = 0; i < 5; i++) {
 *         *(a + i) = i + 1;
 *     }
 *
 *     return a;
 * }
 *
 */
int* dynamicArray(int n, int queries_rows, int queries_columns, int** queries, int* result_count) {
    //n number of empty arrays to init
    //quaries[q] query strings 
    printfdbg("dA input n%d q_r%d q_c%d res%d\n",n,queries_rows,queries_columns,*result_count);
    int** arr = calloc(n,sizeof(int*)); // declare alloc and init    
    int* arrsizes = calloc(n,sizeof(int)); // declare alloc and memset(arrsizes,0,n*sizeof(int));
    unsigned int lastAnswer=0;
    int* answerarr = malloc(queries_rows*sizeof(int)); // declare and alloc to max - max all queries, final comes from query2 count
        
    for(int i=0;i<queries_rows;i++){
        unsigned int idx,x,y;//for formulas
        if(queries[i][0]==1){
            //query 1 x y     
            x = queries[i][1];
            y = queries[i][2];
            idx = ((x^lastAnswer)%n);
            printfdbg("query1 x%d y%d idx%d\n",x,y,idx);
            //append y to arr[idx] => append y to int* a[]
            arrsizes[idx]++;
            int newsize = arrsizes[idx];
            printfdbg("query1 newsize%d arr[%d]%p arrsizes[%d]%d\n",newsize,idx,arr[idx],idx,arrsizes[idx]);
            int* save = realloc(arr[idx], newsize);
            save[newsize-1] = y;
            arr[idx] = save;
            printfdbg("query1 arr[%d]%p\n",idx,arr[idx]);
        }
        else if(queries[i][0]==2){
            //query 2 x y 
            x = queries[i][1];
            y = queries[i][2];
            idx = ((x^lastAnswer)%n);
            printfdbg("query2 x%d y%d idx%d arrsizes[%d]%d\n",x,y,idx,idx,arrsizes[idx]);
            
            lastAnswer = arr[idx][y%arrsizes[idx]];
            
            //answerarr is 0 indexed answerarr[0] if first result
            answerarr[*result_count] = lastAnswer;
            //result count++ yet one more answer saved
            (*result_count)++;
        }
        else {
            printf("error query not handled in dynamicArray\n");            
        }    
          
    }
    
    //shrink answerarr via realloc corrupt results  
    //int* newanswerarr = realloc(answerarr, (size_t) (*result_count));
    //answerarr = newanswerarr;
    free(arrsizes);
    free(arr);
    return answerarr;
}

typedef int* (* f_ptr)(int n, int queries_rows, int queries_columns, int** queries, int* result_count);

int main()
{
    f_ptr dAptr = dynamicArray;// redundant just for play
    FILE* finput = fopen("./tc2.input", "r");
    FILE* foutput = fopen("./tc2.output", "w");

    //char** first_multiple_input = split_string(rtrim(readline()));

    int n ;//= parse_int(*(first_multiple_input + 0));

    int q ;//= parse_int(*(first_multiple_input + 1));

    if (finput) {
        fscanf(finput,"%d %d",&n,&q);
        printfdbg("read from tc2.input n%d q%d\nreading queries\n",n,q);
    }
    else return -1;

    int** queries = malloc(q * sizeof(int*));

    for (int i = 0; i < q; i++) {
        int* p_query = malloc(3 * (sizeof(int)));

        //int qT,qX,qY;
        //fscanf(finput,"%u %u %u",&qT,&qX,&qY);
        fscanf(finput,"%u %u %u",&p_query[0],&p_query[1],&p_query[2]);
        
        *(queries + i) = p_query;
        //printfdbg("query read and saved from finput qT%d qX%d qY%d\n",p_query[0],p_query[1],p_query[2]);
    }

    int result_count;
    int* result = dAptr(n, q, 3, queries, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(foutput,"%d", *(result + i));

        if (i != result_count - 1) {
            fprintf(foutput,"\n");
        }
    }

    //fprintf(finput, "\n");

    fclose(finput);
    fclose(foutput);

    return 0;
}
