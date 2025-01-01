//#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define myDEGPRINF
#ifdef myDEGPRINF
#define printfdbg(fmt, args...)    printf(fmt, ## args)
#else
#define printfdbg(fmt, args...)    /* Don't do anything in release builds */
#endif



//check__two_words
int check_if_anagram(const char* worda, const char* wordb){
    int result = 0;//not the anagram
    //26 of letters in latin dict 
#define ALPHABETSIZEPLUSERROR 27
    int worda_array[ALPHABETSIZEPLUSERROR]={0};
    int wordb_array[ALPHABETSIZEPLUSERROR]={0};

    //if lengs is unqual return result 0
    int length = strlen(worda);
    if(length != strlen(wordb)){
        printfdbg("dbg worda and wordb found to be of diff length at check_if_anagram\n");
        return result;
    }
    //if words are the same return result 0
    if(memcmp(worda,wordb,length)==0){
        printfdbg("dbg worda and wordb found to be the same at check_if_anagram\n");
        return result;
    }

    for( int widx = 0 ; widx < length ; widx++){
        char lidx = 0;

        //calc lidex for arrays , consider A and a got same index
        lidx = worda[widx];
        if((lidx >= 'a') && (lidx <= 'z')) lidx-='a';
        else if ((lidx >= 'A') && (lidx <= 'Z')) lidx-='A';
        else {
            lidx = (ALPHABETSIZEPLUSERROR - 1);//last entry for error chars
        }

        //incr letter count
        worda_array[lidx]++; 

        lidx = wordb[widx]; // code duplication => dedicated function make_index
        if((lidx >= 'a') && (lidx <= 'z')) lidx-='a';
        else if ((lidx >= 'A') && (lidx <= 'Z')) lidx-='A';
        else {
            lidx = (ALPHABETSIZEPLUSERROR - 1);//last entry for error chars
        }
        //incr letter count
        wordb_array[lidx]++; 
    }

    if(worda_array[ALPHABETSIZEPLUSERROR-1] != 0) {
        printfdbg("dbg unexpected chars at dictionary \n");
        return result;
    }

    //check if worda_array and wordb_array are the same
    //FIXME: USER and user should be called same and are not
    if(memcmp(worda_array,wordb_array,ALPHABETSIZEPLUSERROR)==0){
        printfdbg("dbg worda and wordb found to be anagrams \n");
        result = 1;        
    }

    return result;
}

//find_agram_in_dict
void find_print_anagrams(const char* input_word){

    FILE* dict = fopen("./my_dict.txt","r");    
#ifdef _GNU_SOURCE
    //getline when #define _GNU_SOURCE
    //FIXME: this reads extra \n from dictionary 
    char *word = NULL;
    size_t len = 0;
    
    while( getline(&word, &len, dict) != -1){
        //strip out tailing \n 
        if (word[strlen(word)-1]== '\n') word[strlen(word)-1]=0;
        printfdbg("dbg line %s len %d strlen %d\n",word,len,strlen(word));
        if (check_if_anagram((const char*)word,input_word)){
            printf("found %s to be anagram of %s \n",word,input_word);
        }

    }
    
    free(word);
#else
    //fgets
    char buff[80];
    while(fgets(buff,80,dict) != NULL){
        printfdbg("dbg buff %s len %d strlen %d\n",buff,strlen(buff));
        //strip out tailing \n 
        if (buff[strlen(buff)-1]== '\n') buff[strlen(buff)-1]=0;
        if (check_if_anagram((const char*)buff,input_word)){
            printf("found %s to be anagram of %s \n",buff,input_word);
        }
    }
#endif
    //close dict 
    fclose(dict);
}

int main(){

    //input words to find anagrams
    const char* input[3] = {"general","game","user"};

    for (int w = 0; w < 3; w++)
    {
        //get_word_from input
        const char *input_word = input[w];
        //find and pring anagrams from dict
        find_print_anagrams(input_word);
    }    

    char my_input[21];
    printf("enter word :");
    scanf("%20s", my_input);
    find_print_anagrams(my_input);

    return 0;
}
