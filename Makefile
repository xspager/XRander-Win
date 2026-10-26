CFLAGS=-Wall -Wextra -lX11 -lXrender -ggdb -O2

.PHONY:	clean all

all: main

main:	main.c
	$(CC) -o main main.c $(CFLAGS)

clean:
	rm main
