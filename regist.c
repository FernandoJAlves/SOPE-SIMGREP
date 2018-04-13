#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int fd;
struct timeval begin,stop;
char * path;

void initReg(){
	gettimeofday(&begin,NULL);
	path = getenv("LOGFILENAME");
	if(path != NULL){
		fd=open(path,O_WRONLY|O_APPEND|O_CREAT,0644);

	}
	else{
		fd = 0;
		path = NULL;
	}
}

void writeAction(char action[]){
	gettimeofday(&stop,NULL);
	if(fd != 0){
		char temp[256];
		double inst = 1000*(stop.tv_sec - begin.tv_sec) + (stop.tv_usec - begin.tv_usec)/1000;
		sprintf(temp,"%.*f - %d - %s\n",2,inst,getpid(),action);
		write(fd,temp,strlen(temp));
	}
}

void writeInitialCom(int argc, char * argv[]){
	char action[100];
	strcpy(action,"COMANDO grep");
	for(int i = 1; i < argc;i++){
		strcat(action," ");
		strcat(action,argv[i]);
	}
	writeAction(action);
}

void writeFileAction(char c, char file[]){
	char action[100];
	if(c == 'a'){
		strcpy(action,"ABERTO ");
	}
	else if(c == 'f'){
		strcpy(action,"FECHADO ");
	}
	strcat(action,file);
	writeAction(action);
}

char * getPath(){
	return path;
}