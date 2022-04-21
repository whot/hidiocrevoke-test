CC=gcc

revoke: revoke.c
	$(CC) -Wall -Werror -o $@ $^

clean:
	rm -f revoke

all: revoke

