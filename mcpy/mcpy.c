#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>

int test(const void* const data1, const void* const data2, size_t size)
{
    const char* d1 = (char*)data1;
    const char* d2 = (char*)data2;

	for( int i = 0; i < size ; i++){
        //printf("test d1 %c vs d2 %c\n",*d1,*d2);
        if( *d1 != *d2 ) return -1;
        d1++;
        d2++;
    }

    return 0;//equal 
}

void memrandfill(char* s1,long size) {
    //opt for long
    //long idx = 0;

    //long aligned
    const size_t data_oversize = sizeof(long)/sizeof(char);

    while(size % data_oversize != 0){
        char c = (char)rand();
        *s1 = c;
        s1++;
        size--;
    }
    //cast data ptr
    long *ptr_s1 = (long*) s1;
    while (size > data_oversize) {
        long d = (int)rand();//rand is passing ints, assuming 2xints gets into long
        d <<= sizeof(int);
        d |= (int)rand(); 
        // 2x ints saved into long
        *ptr_s1 = d;        
        ptr_s1++;
        size-=data_oversize;
    }
}

void* memcpy1( void *dest, const void *src, size_t count ){
    char* d = (char*) dest;
    const char* s = (char*) src;

    while (count--) {
        *d++ = *s++;
    }
    
    return d;
}

void* memcpy2( void *dest, const void *src, size_t count ){
    char* d = (char*) dest;
    const char* s = (char*) src;

    //align count to 8 x 8 chars  -> long 64 
    while (count % 8 != 0) {
        *d++ = *s++;
        count--;
    }
    //long cpy
    long* ld = (long*) d;
    const long* ls = (long*) s;
    while (count) {
        *ld++ = *ls++;
        count-=8;
    }
    
    d = (char*)ld;
    return d;
}


long utime(long diff) {

    struct timespec tms;

    /* The C11 way */
    if (! timespec_get(&tms, TIME_UTC)) { 

    /* POSIX.1-2008 way */
    //if (clock_gettime(CLOCK_REALTIME,&tms)) {
        return -1;
    }
    /* seconds, multiplied with 1 million */
    long micros = tms.tv_sec * 1000000;
    /* Add full microseconds */
    micros += tms.tv_nsec/1000;
    /* round up if necessary */
    if (tms.tv_nsec % 1000 >= 500) {
        ++micros;
    }
        
    if (diff == 0 ) 
        printf("usec: %u\n",micros);
    else 
        printf("usec: %u\n",micros-diff);

     return micros;
}

//#define FIXSIZE (76)
//#define FIXSIZE (1*1048576)
#define FIXSIZE 1048576 //1M

int main() {
    char s1[FIXSIZE];
    char s2[FIXSIZE];
    char s3[FIXSIZE];
    
    int size = FIXSIZE;

    //printf("ts1 %lu\n", (unsigned long)time(NULL)); 
    long t = utime(0);
    memrandfill(s1,FIXSIZE);
    t = utime(t);
    memcpy1(s2,s1,FIXSIZE);
    t = utime(t);
    memcpy2(s3,s1,FIXSIZE);
    t = utime(t);
    
    printf("test results %d for size %d\n",test(s2,s3,size),size);

    return 0;
}