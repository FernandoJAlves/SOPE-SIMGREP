#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#include "stringFinder.h"



void match_pattern(char str[], char file_path[], options * op)
{
  int fd,r,c=0,j=0;
  char temp,line[100];
  int n = 1;
  char * res;
  char line_temp[256];
  char str_temp[256];

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
        if(op->i){

          convertToUpperCase(line,line_temp);
          convertToUpperCase(str,str_temp);

        }
        else{
          strcpy(line_temp,line);
          strcpy(str_temp,str);
        }
        if(op->w){
          res = isWord(line_temp,str_temp);
        }
        else {
          res = strstr(line_temp,str_temp);
        }

        if(res!=NULL){
          c++;
          if(op->l){
            printf("%s\n",file_path);
            return;
          }
          else if(!op->n && !op->c){
            printf("%s\n",line);
          }
          else if(op->n){
            printf("%d:%s\n",n,line);
          }

        }
        memset(line,0,sizeof(line));
        j=0;
	n++;

      }

    }
  }
  if(op->c){
      printf("%d\n",c);
  }

}

void convertToUpperCase(char sPtr[],char  res[])
{
  strcpy(res, sPtr);
  for(int i = 0; res[i] != '\0';i++) {
    if (islower(res[i])){
      res[i] = toupper(res[i]);
    }
  }

}

char * isWord(char line[], char str[]){
  char * p = line;
  while(1){
    p = strstr(p,str);
    if (p == NULL) return NULL;

    if ((p==line) || (isalnum((unsigned char)p[-1]) == 0)) {
      p += strlen(str);
      if (isalnum((unsigned char)*p) == 0){
        return str;
      }
    }
    // substring was found, but no word match, move by 1 char and retry
    p+=1;
  }
  return NULL;
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
      match_pattern(argv[argc-2],argv[argc-1],&op);

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
