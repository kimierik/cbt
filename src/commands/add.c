
#include "add.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/macros.h"
#include "../public/vector.h"
#include "../public/parser.h"


void handleAdd(char* filename){
    //read the entire file into the vec line by line

    //printf("add start\n");

    FILE *fptr;
    int filesize=0;

    char unvalidatedString[33]; //rn if anything is 32 chars longer  we have buffer overflow
    u8 charpointer=0;// this is the index at the end of unvalidatedStrin
                     // aka this will be the len of unvalidatedStrin

    Vector fileLines=makeVector(sizeof(char**));
    //file represented in lines

    fptr=fopen("cbt.conf", "r");
    //printf("starting to read \n");

    char charb[2];
    while(fgets(charb,2,fptr)){
        //printf("%s\n",charb);
        if (charb[0]=='\n') {
            unvalidatedString[charpointer]=0;//null ternimane

            //printf("adding string  %s\n",unvalidatedString);
            char*str= malloc( sizeof(char*) * charpointer );
            char**strptri=malloc(sizeof(char**));
            //printf(" stringcpy  %s\n",unvalidatedString);
            strcpy(str, unvalidatedString);
            *strptri=str;
            //printf("done %s\n",unvalidatedString);

            pushVector(&fileLines, &strptri);
            filesize+=charpointer;

            charpointer=0;
            //printf("added string \n");
            
        }else{
            unvalidatedString[charpointer++]=charb[0];
        }
    }

    //printf("looping \n");

    //go through all elements in the vector... and stop at FILES_TO_COMPILE
    //then insert the filename into the vec after FILES_TO_COMPILE
    for (int i=0; i<fileLines.size; i++) {
        //printf("is %s\n",**(char***)(fileLines.elements+(i*fileLines.elemSize)) );
        if ( strcmp( **(char***)(fileLines.elements+(i*fileLines.elemSize)) ,"[FILES_TO_COMPILE]" )==0 ){
            //filename is not allocated so that might lead to issues when destroying the vector
            char**strptri=malloc(sizeof(char**));
            *strptri=filename;
            insertAtVector(&fileLines, i, &strptri);
            //printf("%i\n",i);
            break;
        }
    
    }


    //need to empty whatever was in this mem space since we do strcat 
    char buffer[filesize];
    for (int i=0; i<filesize; i++) {
        buffer[i]=0;
    }

    //then make the vec into a single string
    for (int i=0; i< fileLines.size; i++) {
        //for some reasen there is an \t infrom of the first? this is bug
        //printf("%s\n", **(char***)(fileLines.elements+(i*fileLines.elemSize)));

        //why is it not when i print it the first time but when i cat it
        strcat(buffer, **(char***)(fileLines.elements+(i*fileLines.elemSize)));

        strcat(buffer, "\n");
    }
    //printf("file is now\n%s\n",buffer);

    //then overwrite the config with the string
    fclose(fptr);
    fptr=fopen("cbt.conf", "w");
    fputs(buffer, fptr);
    destroyPointerVector(&fileLines, (void(*)(void*))&freeStringPointer);

}


