
#include "init.h"
#include <unistd.h>


#include <stdio.h>


//makes a config file
void handleInit(){
    //check if we allready have a config file
    //      can check with access from unistdh if that is a thing
    if (access("cbt.conf", F_OK)==0) {
        //exists do nothing
        printf("cbt.conf exists nothing written\n");
    
    }else{
        printf("initialising cbt.conf\n");
        //does not exist
        //write some boiler plate into the file
        FILE *fptr= fopen("cbt.conf", "wb");
        fputs("[FILENAME]\n[ENTRYPOINT]\nmain.c\n[FILES_TO_COMPILE]\n[ARGUMENTS]\n-Wall\n[COMPILER]", fptr);
    }
}
