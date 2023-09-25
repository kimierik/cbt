

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

    system("touch cbt.conf");
}

//reads config file and builds the project
void handleBuild(){
    //read config file
    //we need some fns to parse the config propperly

    Config conf =MakeConfig();
    parseConfig(&conf);
    printf("-o %s\n",conf.FILENAME);

    char compilefilesString[100];
    for(int i=0;i<conf.compileListSize;i++){
        strcat(compilefilesString, conf.FILES_TO_COMPILE[i]);
        strcat(compilefilesString, " ");
    }

    char commandbuff[10000];

    snprintf(commandbuff, 10000,"gcc main.c -o %s %s",conf.FILENAME,compilefilesString); 
    //printf("%s\n",commandbuff);

    //todo support multiple comlipers
    system(commandbuff);

}

//calls handle build then runs the program
void handleRun(){}

//creates file and adds it to the config file
void handleCreate(){}
