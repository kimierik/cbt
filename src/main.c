

#include "./commands/build.h"
#include "./commands/run.h"
#include "./commands/add.h"
#include "./commands/init.h"



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./public/parser.h"
#include "commands/generate.h"
#include "public/vector.h"

void handleCreate(char** filenames,int filecount);


//TODO
//new           make multiple dirs and then call init in it basically



void printHelp(){
    printf("\t help page\n");
    printf("\t --help, -h \t displays this help page\n");

    printf("\t init\t initialises cbt config file\n");
    printf("\t create\t makes a file and adds it to the config\n");
    printf("\t build\t builds the project \n");
    printf("\t run\t builds and executes the project \n");

    printf("\t generate\t generates various files (currenly only compile_flags.txt for clang)\n");

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

        if (!strcmp(argv[arg],"generate")){
            handleGenerate();
            return 0;
        }

        if (!strcmp(argv[arg],"add")){
            //todo pass all arguments so we can add other things not just files to compile
            handleAdd(argv[arg+1]);
            return 0;
        }
        if (!strcmp(argv[arg],"create")){
            //todo pass all arguments so we can add other things not just files to compile
            handleCreate(argv,argc);
            return 0;
        }



        printf("no command found for '%s' use --help or -h for instructions\n",argv[arg]);
        return 1;
    }

    printf("\n");
    return 0;
}






//todo change filename into all arguments passed

//creates file and adds it to the config file
void handleCreate(char**filenames,int filecount){
    //loop filenames
    //i is 2 to skip programname and create arguments
    for (int i=2; i<filecount; i++) {
        char* filename= filenames[i];

        //find what extention the files are by looping and finding the '.'
        
        //where the filename ends and the file extention starts
        int startOfExtention=0;
        //go through all chars in filename to find '.'
        
        for(int j=0; j<strlen(filename);j++){
            if(filename[j]=='.'){
                startOfExtention=j;
                break;
            }
        }

        //if we do not find an extention in the filename
        if (startOfExtention==0){
            printf("no extention found for file: %s \n",filename);
            exit(1);
        }

        // then make the files according

        int len=strlen(filename)+10;
        char buffer[len];
        snprintf(buffer, len, "touch %s", filename);
        //printf("%s\n",buffer);
        system(buffer);



        //we need to take 0..startOfExtention of filename and add .h to the end
        snprintf(buffer, len, "touch %.*s.h",startOfExtention, filename);
        //printf("%s\n",buffer);
        system(buffer);

        handleAdd(filename);
    
    }

}




