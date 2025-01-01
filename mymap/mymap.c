// Online IDE - Code Editor, Compiler, Interpreter

#include<stdio.h>
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

typedef struct{
    int map[4][2];
    int keys;
}mymap_t;

void mymap_reset(mymap_t* pm, int key,int val) {
        //reset by setting one key and one value
        pm->map[0][0] = key;
        pm->map[0][1] = val;
        pm->keys = 1; // one key set
    }
void mymap_set(mymap_t* pm, int key,int val){
        pm->map[pm->keys][0] = key;
        pm->map[pm->keys][1] = val;
        if ( pm->keys < 3 ) pm->keys++;
}
void mymap_get(mymap_t* pm, int key,int* val){
    //4 pairs
    for(int i=0;i<pm->keys;i++){
        if(pm->map[i][0] == key) {
            *val = pm->map[i][1];
            return;
        }
    }
}

int main()
{
    mymap_t mymap;
   
    //C1 save only Nend Oend
    int Nend = 3,Nval=5;
    int  Oend=2,Oval=3;
   
    mymap_reset(&mymap,Nend,Nval);
    mymap_set(&mymap,Oend,Oval);
   
    int nval;
    mymap_get(&mymap,max(Nend,Oend),&nval);
    printf("max(Nend,Oend):%d val %d\n",max(Nend,Oend),nval);
   
    //get known key val
    nval = 6677;
    mymap_get(&mymap,3,&nval);
    printf("get known key 3 val 5 : %d\n",nval);
   
    //get known key
    nval = 6677;
    mymap_get(&mymap,2,&nval);
    printf("get known key 2 val 3 : %d\n",nval);
   
    //get unknown key
    nval = 6677;
    mymap_get(&mymap,7,&nval);
    printf("get unknown key val not set : %d\n",nval);
   
    //reset 7, 13
    mymap_reset(&mymap,7,13);
    nval = 6677;
    mymap_get(&mymap,7,&nval);
    printf("after reset key 7,13 get 7 val 13 : %d\n",nval);
   
    //old keys should be unknown after reset
    nval = 6677;
    mymap_get(&mymap,3,&nval);
    printf("after reset key 3 val : %d\n",nval);

    //old keys should be unknown after reset
    nval = 6677;
    mymap_get(&mymap,2,&nval);
    printf("after reset key 2 val : %d\n",nval);

   
   
   
    return 0;
}


