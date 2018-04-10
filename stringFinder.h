#ifndef STRINGFINDER_H_
#define STRINGFINDER_H_

typedef struct{
	int i;
	int l;
	int n;
	int c;
	int w;
	int r;
} options;

void match_pattern(char str[], char file_path[],options * op);

int stringFinder(int argc,char *argv[]);

void convertToUpperCase(char sPtr[],char  res[]);

char * isWord(char line[], char str[]);

#endif
