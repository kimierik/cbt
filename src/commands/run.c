
#include "run.h"

#include "build.h"
#include "../public/parser.h"

#include <stdio.h>
#include <stdlib.h>


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
