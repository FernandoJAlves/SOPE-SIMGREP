#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h> 
#include <sys/wait.h>

#include "stringFinder.h"
#include "regist.h"

#define LENGTH 100

void match_pattern_stdin(char str[],options * op){
  int r,c=0;
  char line[LENGTH];
  int n = 1;
  char * res;
  char line_temp[256];
  char str_temp[256];

  while((r = read(STDIN_FILENO,line,LENGTH)))
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
        printf("%s\n",line);
        printf("(standard input)\n");
        return;
      }
      else if(!op->n && !op->c){
        
          printf("%s",line);
      
      }
      else if(op->n){
        
          printf("%d:%s",n,line);
        

      }

    }
    memset(line,0,sizeof(line));
    n++;


  }
  if(op->c){
    printf("%d\n",c);
  }

}

void match_pattern(char str[], char file_path[], options * op)
{
  int fd,r,c=0,j=0;
  char temp,line[256];
  int n = 1;
  char * res;
  char line_temp[256];
  char str_temp[256];
  writeFileAction('a',file_path);
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
            if(op->r){
              printf("%s:%s\n",file_path,line);
            }
            else{
              printf("%s\n",line);
            }
          }
          else if(op->n){
            if(op->r){
              printf("%s:%d:%s\n",file_path,n,line);
            }
            else{
              printf("%d:%s\n",n,line);
            }
            
          }

        }
        memset(line,0,sizeof(line));
        j=0;
	n++;

      }

    }
  }
  else{
    return;
  }
  if(op->c){
      printf("%d\n",c);
  }
  writeFileAction('f',file_path);
  close(fd);

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

void directory_finder(char str[], char path[],options * op){
  DIR * dir;
  struct dirent *dentry;
  struct stat stat_entry; 
  pid_t pids[64];
  unsigned int n = 0;

  if ((dir = opendir(path)) == NULL) {
    perror(path);
    exit(2);
  }
  writeFileAction('a',path);

  while ((dentry = readdir(dir)) != NULL) {
    char new_path[256];
    strcpy(new_path,path);
    strcat(new_path,dentry->d_name);
    stat(new_path, &stat_entry);

    if(dentry->d_name[0] == '.' || (strcmp(dentry->d_name,"simgrep") == 0)) {
      continue;
    }
    if (S_ISREG(stat_entry.st_mode)) {
      match_pattern(str,new_path,op);
    }
    else if(S_ISDIR(stat_entry.st_mode)){
      strcat(new_path,"/");
      pids[n] = fork();
      n++;
      if(pids[n-1] > 0){
        continue;
      }
      else if(pids[n-1] == 0){
        directory_finder(str,new_path,op);
        break;
      }
      else{
        exit(1);
      }
      
    }
  }
  for(unsigned int i = 0; i < n;i++){
    waitpid(pids[i],NULL,0);
  }
  writeFileAction('f',path);
  closedir(dir);
}

int stringFinder(int argc,char *argv[]){
  struct stat stt;
  options op = {0,0,0,0,0,0,0,0,0};
  setOptions(argc, argv, &op);
  if(argc > 1){

    if(stat(argv[argc-1],&stt)==0){
      if(S_ISREG(stt.st_mode)){
        match_pattern(argv[argc-2],argv[argc-1],&op);
      }
      else if(S_ISDIR (stt.st_mode)){
        if(op.r){
          directory_finder(argv[argc-2],argv[argc-1],&op);
        }
        else{
          perror("The path given is a directory");
          exit(1);
        }
      }
    }
    else{
      if(op.r){
        directory_finder(argv[argc-1],"./",&op);
      }
      else{
        match_pattern_stdin(argv[argc-1],&op);
      }
    }
  }
  else{
    perror("Wrong number of arguments");
    exit(1);
  }
  return 0;
}
