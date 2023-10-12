
#include "build.h"
#include "../public/vector.h"
#include "../public/parser.h"


#include <string.h>
#include <stdlib.h>
#include <stdio.h>


//i dont know where this function should be in
//makes a vector of char*'s int o a single char*
//the char* is mallocced and the caller must take care of it
char* _collapseVecIntoString(Config *conf){
    //we probably add all the things (libs, other args etc) into a single large string

    //the size of the arguments 
    //conf->totalStringSize + ...
    
    char* arguments= malloc(sizeof(char*)*conf->totalStringSize);

    for(int i=0;i<conf->FILES_TO_COMPILE.size;i++){
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


    return arguments;
}


int handleBuild(){

    Config conf =MakeConfig();
    parseConfig(&conf);
    //printf("-o %s\n",conf.FILENAME);

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
