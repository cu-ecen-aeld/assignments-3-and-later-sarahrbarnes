
.PHONY: all
all: writer

writer.o: writer.c
	$(CC) $(CCFLAGS) -c writer.c

writer: writer.o
	$(CC) $(LDFLAGS) writer.o -o writer

# ifdef CROSS_COMPILE
# 	$(CROSS_COMPILE)gcc -g -Wall -c -o writer.o writer.c
# 	$(CROSS_COMPILE)gcc -g -Wall -I/ writer.o -o writer
# else
# 	gcc -g -Wall -c -o writer.o writer.c
# 	gcc -g -Wall -I/ writer.o -o writer
# endif

.PHONY: clean
clean:
	rm -f writer writer.o
