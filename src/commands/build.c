
#include "build.h"
#include "../public/vector.h"
#include "../public/parser.h"


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>


//i dont know where this function should be in
//makes a vector of char*'s int o a single char*
//the char* is mallocced and the caller must take care of it
char* _collapseVecIntoString(Config *conf){
    //we probably add all the things (libs, other args etc) into a single large string

    //the size of the arguments 
    //conf->totalStringSize + ...
    
    char* arguments= malloc(sizeof(char*)*conf->totalStringSize);
    //since we are using strcat we need to know that the list is totally empty

    for (int i =0; i<conf->totalStringSize;i++){ arguments[i]=0;}



    for(int i=0;i<conf->FILES_TO_COMPILE.size;i++){
        //still valid here
        strcat(arguments, **(char***)(conf->FILES_TO_COMPILE.elements+(conf->FILES_TO_COMPILE.elemSize*i)));
        strcat(arguments, " ");
    }
    //printf("%s\n",arguments);


    for(int i=0;i<conf->LIBRARIES.size;i++){
        strcat(arguments, "-l");
        strcat(arguments, **(char***)(conf->LIBRARIES.elements+(conf->LIBRARIES.elemSize*i)));
        strcat(arguments, " ");
    }
    //printf("%s\n",arguments);

    for(int i=0;i<conf->ARGUMENTS.size;i++){
        strcat(arguments, **(char***)(conf->ARGUMENTS.elements+(conf->ARGUMENTS.elemSize*i)));
        strcat(arguments, " ");
    }
    //printf("%s\n",arguments);

    for(int i=0;i<conf->CBTLIBRARIES.size;i++){
        strcat(arguments, "-I");
        strcat(arguments, **(char***)(conf->CBTLIBRARIES.elements+(conf->CBTLIBRARIES.elemSize*i)));
        strcat(arguments, " ");
    }

    return arguments;
}

//copied from parser
//this fn should probably be in the config .h and c
void __addsToCompileList(Config *conf, char *string){
    
    int stlen= strlen(string);
    //allocate string on the heap
    char*str= malloc( sizeof(char*) * stlen );
    strcpy(str, string);

    conf->totalStringSize+=stlen+1; //add strings size to total string size (+1 is for whitespace)

    // vector is a list of pointers. so we need to make a pointer to the string. we cannot place strings on the vector

    char** strptr=malloc(sizeof(char*));
    //printf("%p is %s stris:%p\n",strptr, str,str);
    //something scetchy is happen
    *strptr=str;
    //but something work so something right is happen also
    
    
    //push it on to the vector
    pushVector(&conf->FILES_TO_COMPILE, &strptr);

}



void __readCbtIniFile(const char* path, Config* conf){




    if(strlen(path+13)>=32){
        printf("filesize for %s/cbt_lib.ini is too long max is 32 chars long\n",path);
        exit(1);
    }

    char fullPath[32];
    //just in case empty the array
    for (int i=0;i<31;i++){ fullPath[i]=0; }
    
    snprintf(fullPath, strlen(path)+13, "%s/cbt_lib.ini", path);
    

    FILE *fptr = fopen(fullPath,"r");
    if(fptr==NULL){
        printf("error opening  %s/cbt_lib.ini\n",path);
        exit(1);
    }

    char buffer[2];

    char unvalidatedString[32];

    char fullFilePath[strlen(path)+32];
    //clear it just in case
    for (int i=0;i<strlen(path)+31;i++){ fullFilePath[i]=0; }
    strcpy(fullFilePath, path);
    strcat(fullFilePath, "/");


    int strsize=0;

    while(fgets(buffer,2,fptr)){
        if(buffer[0]=='\n'){
            unvalidatedString[strsize]=0;//null terminate
            //we also need to prefix the string with the given path
            strcat(fullFilePath, unvalidatedString);
            
            __addsToCompileList(conf, fullFilePath);
            strsize=0;
        }else{
            unvalidatedString[strsize++]=buffer[0];
        }

    }
    
    fclose(fptr);

}



int handleBuild(){

    Config conf =MakeConfig();
    parseConfig(&conf);
    //printf("-o %s\n",conf.FILENAME);


    for (int i=0; i<conf.CBTLIBRARIES.size; i++) {
        //printf("read ini file with path %s\n",**(char***)conf.CBTLIBRARIES.elements+(conf.CBTLIBRARIES.size*i));
        __readCbtIniFile( **(char***)conf.CBTLIBRARIES.elements+(conf.CBTLIBRARIES.size*i) , &conf);
    }

    //read all ini files and append the results into correct things in the config


    char* arguments = _collapseVecIntoString(&conf);

    u32 commandsize=conf.totalStringSize+strlen(conf.FILENAME)+strlen(conf.ENTRYPOINT)+16; //16 is "gcc main.c...."
    char commandbuff[commandsize];

    snprintf(commandbuff, commandsize,"%s %s -o %s %s",conf.COMPILER,conf.ENTRYPOINT,conf.FILENAME,arguments); 
    printf("%s\n",commandbuff);

    //call the built system command
    int returnvalue = system(commandbuff);
    
    //TODO CLEAR EVERYTHING IN CONFIG VECTORS 
    //rn the program dies after this is done so we dont really need to worry about anythign
    freeConfigVectors(&conf);
    return returnvalue;
}
