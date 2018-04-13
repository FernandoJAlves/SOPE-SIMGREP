#include "stringFinder.h"
#include "termhandler.h"
#include "regist.h"


int main(int argc,char *argv[]) {
	initReg();
	writeInitialCom(argc,argv);
	term_conf();
	stringFinder(argc,argv);
	return 0;
}