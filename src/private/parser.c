
//read the file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/macros.h"
#include "../public/parser.h"


//TODO
//change signifiers variable name to something understandable







//make conf
Config MakeConfig(){
    struct Config conf;
    conf.compileListSize=0;
    strcpy(conf.FILENAME, "a.out");
    return conf;
}




//returns the index of the signifier from the list
//returns 0 if is not sig
int GetSigId(char* string){
    char* signifiers[]= {"[FILENAME]","[FILES_TO_COMPILE]"};
    for (int i=0;i<2;i++) {
        if (strcmp(signifiers[i], string)==0) {
            return i+1;
        }
    }
    return 0;
}


//uhhh
void __addToStrList(Config *conf, char *string){
    strcpy(conf->FILES_TO_COMPILE[conf->compileListSize++], string);
}





void parseConfig(Config* conf){
    //open file



    FILE* fptr;

    fptr=fopen("cbt.conf","r");
    if (fptr==NULL) {
        printf("found no config file cbt.conf\n");
        exit(2);
    }

    //wait how the fuck do we know anythign about the len of the buffer
    //do we read a single char at a time and try to parse untill we hit \n
    
    //do we make a dynamic array thing
    //append chars to that untill we hit \n then try to figure out what it means

    //DANGER DANGER DANGER 
    char unvalidatedString[33]; //rn if anything is 32 chars longer  we have buffer overflow
    u8 charpointer=0;// this is the index at the end of unvalidatedStrin
                     // aka this will be the len of unvalidatedStrin

    int sigId=0;
    int TempSigId=0;


    char charb[2];
    while(fgets(charb,2,fptr)){
        if (charb[0]=='\n') {
            unvalidatedString[charpointer]=0;//null ternimane
            charpointer=0;
            TempSigId = GetSigId(unvalidatedString);
            //printf("tsig:%i is:%s\n",TempSigId,unvalidatedString);
            if (TempSigId==0) { //>0
                //not signifier
                //
                //assign the string to the approipriate thing
                if(sigId==1){
                    strcpy(conf->FILENAME, unvalidatedString);
                    //printf("assign filename %s\n",unvalidatedString);
                }

                if(sigId==2){
                    __addToStrList(conf, unvalidatedString);
                }



            }else{
                sigId=TempSigId;
                //printf("sigId:%i\n",sigId);
            }

        }else{
            unvalidatedString[charpointer++]=charb[0];
        }
    }


}







