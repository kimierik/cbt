#include "generate.h"
#include "../public/parser.h"
#include <stdio.h>




void handleGenerate(){
    Config conf=MakeConfig();
    parseConfig(&conf);

    //gen the file
    FILE * fptr=fopen("compile_flags.txt","w");

    for (int i=0; i<conf.CBTLIBRARIES.size; i++) {
        fputs("-I\n", fptr);
        fputs(**(char***)conf.CBTLIBRARIES.elements+(conf.CBTLIBRARIES.elemSize*i), fptr);
        fputs("\n", fptr);
    }

    fclose(fptr);
}


