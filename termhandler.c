#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int signo){
	char ans;
	while(1){
		printf("\nAre you sure you want to terminate the program? (Y/N) ");
		scanf("%c",&ans);
		printf("\n");
		if(ans == 'y' || ans == 'Y'){
			exit(0);
		}
		else if(ans == 'n' || ans == 'N'){
			return;
		}
	}
}

void term_conf(){
	struct sigaction action;

	action.sa_handler = sigint_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	if (sigaction(SIGINT,&action,NULL) < 0){
		fprintf(stderr,"Unable to install SIGTERM handler\n");
		exit(1);
	}
}
