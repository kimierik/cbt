#ifndef PARSER
#define PARSER


#include "../utils/macros.h"
#include "./vector.h"

//TODO 
//make FILES_TO_COMPILE into vector
//make filename int pointer and use malloc

typedef struct Config{

    char FILENAME[32];
    char ENTRYPOINT[32];

    Vector FILES_TO_COMPILE; //vec of char**
    u32 totalStringSize; //size of all the strings in the vector combined

    Vector LIBRARIES; //vec of char**

}Config;



void parseConfig(Config* conf);
Config MakeConfig();

//assumes the input string is null terminated
void __addToStrList(Config*conf, char*string);
//wait how the fuck did i imagine this going
//this has to be some memory violation how do we just append strings to somethign with undefines size
//do we just malloc it bigger just implement a vector 





#endif
