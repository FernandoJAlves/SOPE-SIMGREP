SRC = *

all:
	gcc -Wall $(SRC).c -o simgrep

clean:
	rm run
