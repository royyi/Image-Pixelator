#11/4/23
all: example cliv

example: example.c
	gcc -Wall -Wextra -Werror -o example example.c

cliv: cliv.c
	gcc -Wall -Wextra -Werror -o cliv cliv.c

clean:
	rm -f example
