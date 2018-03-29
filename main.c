#include "stringFinder.h"
#include "termhandler.h"
#include <stdio.h>


int main(int argc,char *argv[]) {
	term_conf();
	stringFinder(argc,argv);
	return 0;
}