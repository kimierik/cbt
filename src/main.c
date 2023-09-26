

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./public/parser.h"

void handleInit();
void handleBuild();
void handleRun();
void handleCreate();




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
    printf("%i\n",argc);
    for(int arg=1;arg<argc;arg++){
        printf("%s \n",argv[arg] );

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



//makes a vector of char*'s int o a single char*
//the char* is mallocced and the caller must take care of it
char* _collapseVecIntoString(Config *conf){
    //we probably add all the things (libs, other args etc) into a single large string

    //the size of the arguments 
    //conf->totalStringSize + ...
    
    char* arguments= malloc(sizeof(char*)*conf->totalStringSize);

    for(int i=0;i<conf->FILES_TO_COMPILE.size;i++){
        strcat(arguments, *(char**)(conf->FILES_TO_COMPILE.elements+(conf->FILES_TO_COMPILE.elemSize*i)));
        strcat(arguments, " ");
    }
    //printf("%s\n",arguments);


    for(int i=0;i<conf->LIBRARIES.size;i++){
        strcat(arguments, "-l");
        strcat(arguments, *(char**)(conf->LIBRARIES.elements+(conf->LIBRARIES.elemSize*i)));
        strcat(arguments, " ");
    }
    //printf("%s\n",arguments);

    for(int i=0;i<conf->ARGUMENTS.size;i++){
        strcat(arguments, *(char**)(conf->ARGUMENTS.elements+(conf->ARGUMENTS.elemSize*i)));
        strcat(arguments, " ");
    }
    //printf("%s\n",arguments);


    return arguments;
}


//reads config file and builds the project
void handleBuild(){

    Config conf =MakeConfig();
    parseConfig(&conf);
    //printf("-o %s\n",conf.FILENAME);

    char* arguments = _collapseVecIntoString(&conf);

    u32 commandsize=conf.totalStringSize+strlen(conf.FILENAME)+strlen(conf.ENTRYPOINT)+16; //16 is "gcc main.c...."
    char commandbuff[commandsize];

    snprintf(commandbuff, commandsize,"%s %s -o %s %s",conf.COMPILER,conf.ENTRYPOINT,conf.FILENAME,arguments); 
    printf("%s\n",commandbuff);

    //call the built system command
    system(commandbuff);
    
    //TODO CLEAR EVERYTHING IN CONFIG VECTORS 
    //rn the program dies after this is done so we dont really need to worry about anythign
}

//calls handle build then runs the program
void handleRun(){}

//creates file and adds it to the config file
void handleCreate(){}
