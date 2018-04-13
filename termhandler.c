#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t parent_pid;

void sigint_handler(int signo){
	if(getpid() != parent_pid){
		return;
	}
	char ans;
	while(1){
		printf("\nAre you sure you want to terminate the program? (Y/N) ");
		scanf("%c",&ans);
		printf("\n");
		if(ans == 'y' || ans == 'Y'){
			signal(SIGQUIT, SIG_IGN);
			kill(parent_pid, SIGQUIT);
			exit(0);
		}
		else if(ans == 'n' || ans == 'N'){
			return;
		}
	}
}

void term_conf(){
	parent_pid = getpid();
	struct sigaction action;

	action.sa_handler = sigint_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	if (sigaction(SIGINT,&action,NULL) < 0){
		fprintf(stderr,"Unable to install SIGTERM handler\n");
		exit(1);
	}
}
