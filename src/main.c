

#include "./commands/build.h"
#include "./commands/run.h"
#include "./commands/add.h"



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./public/parser.h"
#include "public/vector.h"

void handleInit();
void handleCreate(char* filename);


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





//creates file and adds it to the config file
void handleCreate(char*filename){
    int len=strlen(filename)+10;
    char buffer[len];
    snprintf(buffer, len, "touch %s", filename);
    system(buffer);
    //this would make .cpp.h withc is not what i want but i gotta figure it out later
    snprintf(buffer, len, "touch %s.h", filename);
    system(buffer);

    handleAdd(filename);
}




