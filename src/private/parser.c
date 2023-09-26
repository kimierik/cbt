
//read the file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/macros.h"
#include "../public/parser.h"


//TODO
//change signifiers variable name to something understandable







//make conf
Config MakeConfig(){
    struct Config conf;
    conf.totalStringSize=0;
    conf.FILES_TO_COMPILE= makeVector(sizeof(char**));
    conf.LIBRARIES=        makeVector(sizeof(char**));
    conf.ARGUMENTS=        makeVector(sizeof(char**));
    strcpy(conf.FILENAME, "a.out");
    strcpy(conf.ENTRYPOINT, "main.c");
    strcpy(conf.COMPILER, "g++");
    return conf;
}




//returns the index of the signifier from the list
//returns 0 if is not sig
int GetSigId(char* string){
    char* keywords[]= {"[FILENAME]","[FILES_TO_COMPILE]","[LIBRARIES]","[ENTRYPOINT]","[ARGUMENTS]","[COMPILER]"};
    int size=sizeof(keywords)/sizeof(keywords[0]);
    //printf("sigs size is %i",size);
    for (int i=0;i<size;i++) {
        if (strcmp(keywords[i], string)==0) {
            return i+1;
        }
    }
    return 0;
}


//assumes string is NOT allocated on the heap
//this function allocates string on the heap with malloc
void __addToCompileList(Config *conf, char *string){
    
    int stlen= strlen(string);
    //allocate string on the heap
    char*str= malloc( sizeof(char*) * stlen );
    strcpy(str, string);

    conf->totalStringSize+=stlen+1; //add strings size to total string size (+1 is for whitespace)

    // vector is a list of pointers. so we need to make a pointer to the string. we cannot place strings on the vector
    char** strptr=malloc(sizeof(char*));
    *strptr=str;
    
    
    //push it on to the vector
    pushVector(&conf->FILES_TO_COMPILE, strptr);

}

void __addToLibraryList(Config *conf, char *string){
    
    int stlen= strlen(string);
    //allocate string on the heap
    char*str= malloc( sizeof(char*) * stlen );
    strcpy(str, string);

    conf->totalStringSize+=stlen+3; //add strings size to total string size (+3 is for whitespace+"-l")

    // vector is a list of pointers. so we need to make a pointer to the string. we cannot place strings on the vector
    char**strptr=malloc(sizeof(char*));
    *strptr=str;
    
    
    //push it on to the vector
    pushVector(&conf->LIBRARIES, strptr);

}

void __addToArgumentList(Config *conf, char *string){
    
    int stlen= strlen(string);
    //allocate string on the heap
    char*str= malloc( sizeof(char*) * stlen );
    strcpy(str, string);

    conf->totalStringSize+=stlen+1; //add strings size to total string size (+1 is for whitespace)

    // vector is a list of pointers. so we need to make a pointer to the string. we cannot place strings on the vector
    char** strptr=malloc(sizeof(char*));
    *strptr=str;
    
    
    //push it on to the vector
    pushVector(&conf->ARGUMENTS, strptr);

}




void parseConfig(Config* conf){
    //open file



    FILE* fptr;

    fptr=fopen("cbt.conf","r");
    if (fptr==NULL) {
        printf("found no config file cbt.conf\n");
        exit(2);
    }

    //wait how the fuck do we know anythign about the len of the buffer
    //do we read a single char at a time and try to parse untill we hit \n
    
    //do we make a dynamic array thing
    //append chars to that untill we hit \n then try to figure out what it means

    //DANGER DANGER DANGER 
    char unvalidatedString[33]; //rn if anything is 32 chars longer  we have buffer overflow
    u8 charpointer=0;// this is the index at the end of unvalidatedStrin
                     // aka this will be the len of unvalidatedStrin

    int keywordId=0;
    int TempSigId=0;


    char charb[2];
    while(fgets(charb,2,fptr)){
        if (charb[0]=='\n') {
            unvalidatedString[charpointer]=0;//null ternimane
            charpointer=0;
            TempSigId = GetSigId(unvalidatedString);
            //printf("tsig:%i is:%s\n",TempSigId,unvalidatedString);
            if (TempSigId==0) { //>0
                //not signifier
                //
                //assign the string to the approipriate thing
                //[FILENAME]
                if(keywordId==1){
                    strcpy(conf->FILENAME, unvalidatedString);
                    //printf("assign filename %s\n",unvalidatedString);
                }

                //[FILES_TO_COMPILE]
                if(keywordId==2){
                    //printf("adding filename %s\n",unvalidatedString);
                    __addToCompileList(conf, unvalidatedString);
                }

                //[LIBRARIES]
                if(keywordId==3){
                    __addToLibraryList(conf, unvalidatedString);
                }

                //[ENTRYPOINT] (aka main.c or main.cpp etc)
                if(keywordId==4){
                    strcpy(conf->ENTRYPOINT, unvalidatedString);
                }

                //[ARGUMENTS]
                if(keywordId==5){
                    //printf("adding filename %s\n",unvalidatedString);
                    __addToArgumentList(conf, unvalidatedString);
                }

                //[COMPILER] (aka main.c or main.cpp etc)
                if(keywordId==6){
                    strcpy(conf->COMPILER, unvalidatedString);
                }



            }else{
                keywordId=TempSigId;
            }

        }else{
            unvalidatedString[charpointer++]=charb[0];
        }
    }


}







