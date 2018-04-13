#ifndef REGIST_H_
#define REGIST_H_

void initReg();

void writeAction(char action[]);

void writeInitialCom(int argc, char * argv[]);

void writeFileAction(char c, char file[]);

char * getPath();

#endif