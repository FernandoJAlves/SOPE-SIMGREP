#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "stringFinder.h"



void match_pattern(char str[], char file_path[])
{
    int fd,r,j=0;
    char temp,line[100];

    if((fd=open(file_path,O_RDONLY)) != -1)
    {

        while((r=read(fd,&temp,sizeof(char)))!= 0)
        {

            if(temp!='\n')
            {
                line[j]=temp;
                j++;
            }
            else
            {
              //printf("%s\n",line);
                if(strstr(line,str)!=NULL){
                  printf("%s\n",line);
                };
                memset(line,0,sizeof(line));
                j=0;

            }

        }
    }
}


void setOptions(int argc, char *argv[],options * op){
    for(int i = 1; i < argc; i++){
        if(*argv[i] == '-'){
            for(char * c = argv[i];*c != '\0'; c++){

                if(*c == 'i'){
                    op->i = 1;
                }

                if(*c == 'l'){
                    op->l = 1;
                }

                if(*c == 'n'){
                    op->n = 1;
                }

                if(*c == 'c'){
                    op->c = 1;
                }

                if(*c == 'w'){
                    op->w = 1;
                }

                if(*c == 'r'){
                    op->r = 1;
                }
            }
        }

    }
}

int stringFinder(int argc,char *argv[])
{
    struct stat stt;
    options op = {0,0,0,0,0,0};
    setOptions(argc, argv, &op);
    if(argc >= 3)
    {
        if(stat(argv[argc-1],&stt)==0){
            match_pattern(argv[argc-2],argv[argc-1]);

          }
        else
        {
            perror("stat()");
            exit(1);
        }
    }
    else{
        perror("Wrong number of arguments");
        exit(1);
    }

    return 0;
}
