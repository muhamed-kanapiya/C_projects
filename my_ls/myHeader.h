#include <stdbool.h>
#include<time.h>


//for open/read/closedir and stat - list fnx
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>






//create structure containing name and time modification
//to store info we need:
    //name, type (if dir => for recursion) and modified time
typedef struct metaDataAboutFiles {
    char* name; // from readdir
    char* statName;
    _Bool isDir; //0 if file, 1 if dir  //from stat
    time_t time; //from stat
    _Bool isVis; // is visible
} meta;

typedef struct struct_with_whole_data_and_metadata_about_them{
    _Bool isDir;
    int dataSize;
    int visSize;
    int dirVisSize;
    int dirSize;
    int arrSize;
    char* cwd;
    meta* arr;
} inf;


//declaration of fnx in header
inf* list(char* dir);

void printFiles(char* paths, char* mode);

int alpha(const void *a, const void *b);

int byTime(const void *a, const void *b);

int my_tipa_getopt(int argc, char* argv[], char* mode, char** dirs);