#include <string.h>
#define MULTARG mode[3]
#define TIMESORT mode[2]
#define HIDDEN mode[1]
#define RECUR mode[0]
#define NONE_MODE 'x'

#define BUFFSIZE 500
#define NAMESIZE 50
#define PATHNAME 200
#define ARRAYSIZE 50

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//my headers -- struct/fnx declarations
#include "myHeader.h"


int main(int argc, char* argv[])
{
                // Rat
    char mode[] = "xxxx";
    
    //parse commandline arguments
    //changes mode accordingly // returns dirs - only non flag commandlines
    //int count = items in dirs
    char** dirs = calloc(argc == 1 ? 1: argc-1, sizeof(char*));

    int count = my_tipa_getopt(argc, argv, mode, dirs);

    if(count==0){
        //no paths given by commandline
        dirs[0]=".";
        count = 1;
    }else if(count>1)
        //multiple paths given?
        MULTARG = 'm';
    
    //now loop though commands and print contents
    for(int i=0; i<count; i++){
        printFiles(dirs[i], mode);
    }

    free(dirs);
    

    return 0;
}


//parses commandline args
//changes mode accordingly (RatM)  recurs/hidden/by time/multipleDirs
//changes dirs - contain only nonflag args (paths)
//returns number of items in dirs
int my_tipa_getopt(int argc, char* argv[], char* mode, char** dirs){

    //to get flags of commandline
    char* errmess = "Option %c should be given only once\n";
    char* usage = "Usage:\t%s [-Rat] [optDir [optDir2]]]\n";
    char opt;

    //counter + indexer for dirs
    int counter = 0;


    for(int i = 1; i<argc; i++){
        if( argv[i][0]=='-'){
            for(int j=1; argv[i][j]!='\0'; j++){
                opt = argv[i][j];
                switch( opt ){
                    case 'R':
                        if(RECUR==NONE_MODE)
                            RECUR = 'R';
                        else
                            fprintf(stderr, errmess, opt);
                        break;
                    case 'a':
                        if(HIDDEN==NONE_MODE)
                            HIDDEN = 'a';
                        else
                            fprintf(stderr, errmess, opt);
                        break;
                    case 't':
                        if(TIMESORT==NONE_MODE)
                            TIMESORT = 't';
                        else
                            fprintf(stderr, errmess, opt);
                        break;
                    default:
                        fprintf(stderr, usage, argv[0]);
                        exit(EXIT_FAILURE);
                }
            }
        }else{
            //else of above
            dirs[counter++] = &argv[i][0];
        }
    }

    //return number of dirs in dirs-array
    return counter;
}




//sorting function used in qsort()
//sorts alphabetically
int alpha(const void *p1, const void *p2){
    meta* a = (meta*) p1;
    meta*b = (meta*) p2;
    int result;

    //files are sorted before dirs
    if(a->isDir!=b->isDir){
        if(a->isDir)
            result = 1;
        else
            result = -1;
    
    }else if(a->isVis!=b->isVis){
        //hidden before visible
        if(a->isVis)
            result = 1;
        else
            result = -1;

    }else{
        //if both are file/dir => alphabetical order
        result = strcmp(a->name, b->name);
    }

    return result;
}



//sorting function used in qsort()
//sorts by time
int byTime(const void *p1, const void *p2){
    meta* a = (meta*) p1;
    meta*b = (meta*) p2;
    int result;

    //same as alpha
    //files are sorted before dirs
    if(a->isDir!=b->isDir){
        if(a->isDir)
            result = 1;
        else
            result = -1;
    
    }else if (a->time!=b->time){
        //newly added
        if(a->time>b->time)
            result = -1;
        else
            result = 1;
    }else if(a->isVis!=b->isVis){
        //hidden before visible
        if(a->isVis)
            result = 1;
        else
            result = -1;

    }else{
        //if both are file/dir => alphabetical order
        result = strcmp(a->name, b->name);
    }

    return result;
}



//wrapper fnx
//takes path and prints contents
//if header true => prints directory name
void printFiles(char* path, char* mode){
    //to avoid changing string literal cpy path to paths
    int len = strlen(path)+1;
    char paths[len];
    strcpy(paths, path);

    //mode = "abc"
    //a - recursion, b - hidden, c - bytime
    //xxx - all three negated

    //it has exit() fnxs => call it before calloc
    inf* data = list(paths);

    //placeHolder var   //create buff char containing
    char tmp[NAMESIZE], *buff = calloc(BUFFSIZE, sizeof(char));

    if(data->isDir==false){
        strcpy(buff, data->arr[0].name);
        goto endPrint;
    }

    //directory array for recurion
    int recSize = ARRAYSIZE, recCount = 0, recWordSize = NAMESIZE;
    char** recDir;
    if(RECUR!=NONE_MODE){
         recDir = calloc(recSize, sizeof(char*));
        //init/delcare each char* in recDir
            //bc we free data inf struct => so we lose pointer
            //we just need to copy
        for(int i=0; i<recSize; i++){
            recDir[i] = calloc(recWordSize, sizeof(char));
        }
    }

    //if multiple path or recursion mode => print directory name
    if(RECUR!=NONE_MODE || MULTARG!=NONE_MODE){
        sprintf(tmp, "%s:\n", paths);
        strcpy(buff, tmp);
        
    }
    
    //if -t is not set
    //sorting by alpha vs byTime
    if(TIMESORT==NONE_MODE)
        qsort(data->arr, data->dataSize, sizeof(meta), alpha);
    else
        qsort(data->arr, data->dataSize, sizeof(meta), byTime);
    
    
    //printing results
    for(int i=0; i< data->dataSize; i++){
        //list all files in current directory
        if(data->arr[i].isVis){
            sprintf(tmp,"%s\t", data->arr[i].name);
            strcat(buff, tmp);
        }else if(HIDDEN!=NONE_MODE){
            //if hidden and hidden allowed
            sprintf(tmp, "%s\t", data->arr[i].name);
            strcat(buff, tmp);
        }
        //if recursion mode -> save directories for later looping
        if(RECUR!=NONE_MODE && data->arr[i].isDir){
                //if it not hidden and hidden not allowed
                if(data->arr[i].isVis == true){
                    //save  dir names for recursion
                    strcpy(recDir[recCount++], data->arr[i].statName);
                //hidden
                } else if(HIDDEN != NONE_MODE){
                    //skip . and .. to avoid looping in one directory
                    if( strcmp(data->arr[i].name, ".")==0 ) continue;
                    if( strcmp(data->arr[i].name, "..")==0 ) continue;

                    strcpy(recDir[recCount++], data->arr[i].statName);
                }
        }
    }

    endPrint:
    strcat(buff, "\n");
    printf("%s", buff);

    //free from list
    for(int i=0; i<data->arrSize; i++){
        free(data->arr[i].name);
        free(data->arr[i].statName);
    }
    free(data->arr);

    //if -R RECUR flag is give, we need to check whether the
        //file is dir - OK, or file - skip
    _Bool isDir = data->isDir;

    free(data);

    //free buff after printing
    free(buff);
    
    //add new line
    printf("\n");
    

    if(RECUR!=NONE_MODE && isDir){
        //if recursion
        for(int i=0; i<recCount; i++){
            printFiles(recDir[i], mode);
        }

        //if recursive mode => recdir was calloced => in reverse order
        for(int i=0; i<recSize; i++){
            free(recDir[i]);
        }

        free(recDir);
    }

}






//funx takes path and returns made struct pointer with necessary info about each file/dir in path
inf* list(char* path){
    _Bool isDir = true;

    //define vars
    int size = ARRAYSIZE;
    int size_counter = 0;

    //for callocing name char* in arr in inf struct
    int name_size = NAMESIZE;
    int path_size = PATHNAME;



    //get directory pointer
    DIR* dir = opendir(path);

    //check for error on opening directory
    if(dir==NULL){
        //if no access then probably it is a file
        isDir = false;
    }
    //create array of pointers of size
    inf* data = malloc(sizeof(inf));

    //declare and itialize data
    data->dataSize = 0;
    data->visSize = 0;
    data->dirVisSize = 0;
    data->dirSize = 0;
    //is the path itself a file?
    data->isDir = isDir;

    data->arr = calloc(size, sizeof(meta) );
    for(int i=0; i<size; i++){
        data->arr[i].name = calloc(name_size, sizeof(char) );
        //if recursion => need to know path as well as name
        data->arr[i].statName = calloc(path_size, sizeof(char) );
    }
    //save size for freeing
    data->arrSize = size;

    //give relative path to current directory
    data->cwd = path;

    //create placeholders
    struct dirent* placeHolder;
    char tmp[NAMESIZE];

    //make path ready
    strcpy(tmp, path);
    strcat(tmp, "/");

    //if argument is file no need to readdir
    if(isDir==false){
        strcpy(data->arr[0].statName, path);
        //the path could be a path, not a name
        //find if '/' in the string
        char* slash = strrchr(path, '/');
        if(slash==NULL){
            //name does not contain '/'
            strcpy(data->arr[0].name, path);
        }else{
            //as slash points to '/', but we need from next on
            strcpy(data->arr[0].name, ++slash);
        }
        goto file;

    }

    //loop and write contents into arr
    while( (placeHolder=readdir(dir)) != NULL ){

        strcpy(data->arr[size_counter].name, placeHolder -> d_name);
        data->arr[size_counter].isVis = placeHolder -> d_name[0] == '.' ? false : true;
        strcpy(data->arr[size_counter].statName, tmp);
        strcat(data->arr[size_counter].statName, placeHolder -> d_name);

        //if visible: incrememnt counter
        if(data->arr[size_counter].isVis){
            data->visSize += 1;
        }

        //give metadata about size of arr in data and  //set size_counter to next;
        data->dataSize +=1;

        size_counter++;

        //check if arr size is adequate  - - - - - later
        if(size_counter==size){
            size *= 2;
            /*arr = realloc(arr, sizeof(struct dirent*)*size);
            //as size is increased, we need to initialize arrays elements
            for(int i=0; i<size; i++){
            }*/
        }
    }

    //close dir
    if( closedir(dir) != 0 ){
        fprintf(stderr, "Cannot release directory: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    //before accessing file descr via stat
    //create buff
    struct stat buff;
    int ret;

    //loop though names and assign other properties //loop till <counter, bc of [counter++]
    for(int i=0; i<size_counter; i++){
        //special case for file
        if(i==0){
            file: //stepping over dir-stuff
            ret = stat(data->arr[0].statName, &buff);
        }else
            ret = stat(data->arr[i].statName, &buff);
        if(ret!=0){
            perror("Error on access to file: ");
            exit(EXIT_FAILURE);
        }
        //receive info about file modificaton time
        data->arr[i].time = buff.st_mtime;
        //receive info about file type and...
        if( (data->arr[i].isDir = S_ISDIR(buff.st_mode)) ){
            //increment counter is it is dir
            data->dirSize +=1;
            if(data->arr[i].isVis)
                //increment dirVisSize
                data->dirVisSize +=1;
        }
        if(data->isDir==false)
            break;
    }

    return data;
}