

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./public/parser.h"
#include "public/vector.h"

void handleInit();
int handleBuild();
void handleRun();
void handleCreate(char* filename);
void handleAdd(char* filename);


//TODO


//init          write something to the file and not just touch it 
//new           make multiple dirs and then call init in it basically





void printHelp(){
    printf("\t help page\n");
    printf("\t --help, -h \t displays this help page\n");

    printf("\t init\t initialises cbt config file\n");
    printf("\t create\t makes a file and adds it to the config\n");
    printf("\t build\t builds the project \n");
    printf("\t run\t builds and executes the project \n");

    printf("\t new\t \n");


}






int main (int argc, char ** argv){

    //argv is a list of nullterminated strings so that is good

    if (argc==1){
        printHelp();
        return 0;
    }

    //we need to loop through it and see if the command is appropriate
    //printf("%i\n",argc);
    for(int arg=1;arg<argc;arg++){
        //printf("%s \n",argv[arg] );

        if (!strcmp(argv[arg],"--help") || !strcmp(argv[arg],"-h")){
            printHelp();
            return 0;
        }

        if (!strcmp(argv[arg],"init")){
            handleInit();
            return 0;
        }
        if (!strcmp(argv[arg],"build")){
            handleBuild();
            return 0;
        }

        if (!strcmp(argv[arg],"run")){
            handleRun();
            return 0;
        }

        if (!strcmp(argv[arg],"add")){
            //todo pass all arguments so we can add other things not just files to compile
            handleAdd(argv[arg+1]);
            return 0;
        }
        if (!strcmp(argv[arg],"create")){
            //todo pass all arguments so we can add other things not just files to compile
            handleCreate(argv[arg+1]);
            return 0;
        }



        printf("no command found for '%s' use --help or -h for instructions\n",argv[arg]);
        return 1;
    }

    printf("\n");
    return 0;
}


//makes a config file
void handleInit(){
    //check if we allready have a config file
    //if so do nothing rn
    //else make config file

    //todo populate the conf file dont just make an empty one
    system("touch cbt.conf");
}



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


//reads config file and builds the project
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

//calls handle build then runs the program
void handleRun(){
    int exitcode=handleBuild();

    if (exitcode==0) {
        //we parse the file 2 times in a row 
        //this can be optimized
        //expecially since we dont free anything from build rn
        Config conf =MakeConfig();
        parseConfig(&conf);

        //this is blocking idk if we should let cbt die before calling but idk if it matters tbh
        int ret=system(conf.FILENAME);
        if (ret==139){
            printf("Segmentation Fault\n");
        }
        freeConfigVectors(&conf);
    } else {
        printf("\nerror building program did not run\n");
    }

}




//adds filename into the [FILES_TO_COMPILE] section of the config
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



//creates file and adds it to the config file
void handleCreate(char*filename){
    int len=strlen(filename)+10;
    char buffer[len];
    printf("blen %i\n",len);
    snprintf(buffer, len, "touch %s", filename);
    system(buffer);
    handleAdd(filename);
}




