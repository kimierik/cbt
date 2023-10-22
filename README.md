# C Build tool
a simple C build tool. mainly constructs a single command from text in cbt.conf file  

## compilation
to compile this just run the run.sh file  


  
## Useage
### commands
|Command|Description|
|:---|:---|
|--help, -h|  displays this help page|
|init| initialises cbt config file|
|create |makes a file and adds it to the config|
|build| builds the project |
|run| builds and executes the project |
|generate| generates compile_flags.txt|


## cbt.conf
the program reads the cbt.conf file and generates a command to a compiler from its contents  
the cbt.conf file has 6 keywords
 - [FILENAME]  
        the output filename. this translates to the -o  
 - [FILES_TO_COMPILE]  
        list of .c/.c++ files that would need to be compiled seperately  
 - [LIBRARIES]  
        libraries that this program uses.  
        everything in this section is prefixed with -l  
 - [ENTRYPOINT]  
        entrypoint of the program  
        this is the first argument to the command, unlike FILES_TO_COMPILE this is only a single string and cannot accept multiple elements  
 - [ARGUMENTS]  
        normal arguments, elements here are not prefixed with anything  
 - [COMPILER]  
        optional. here you can determine if you want to compile with gcc or g++ if you want to   
 - [CBTLIBS]  
        library system that is handled by the tool itself..   
        this should point to a directory that contains a cbt_lib.ini file  

## cbt libs
cbt libs is a library management system for this tool.  
a simple example of how it works can be found (here)[https://github.com/kimierik/cbt/tree/main/example/cbtlibs]


## TODO/WORK IN PROGRESS
init    -- missing functionality  
new     -- function that makes a new project with propper folder structure and conf file  
create  -- create should also initialise a .h file in approprtiate location but does not  
add     -- should have a way to add command line arguments aswell not just files to compile etc   
remove  -- opposite of add   
delete  -- deletes files and removes them from the config  

