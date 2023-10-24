CFLAGS=-ggdb
all: create show

create:
	gcc create.c db.c utils.c $(CFLAGS) -o $@

show:
	gcc show.c db.c utils.c $(CFLAGS) -o $@

clean:
	rm create show

.PHONY: all clean help
