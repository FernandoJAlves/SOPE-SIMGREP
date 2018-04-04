SRC = *

all:
	gcc -Wall $(SRC).c -o simgrep

debug:
	gcc -g -Wall $(SRC).c -o simgrep

clean:
	rm run
