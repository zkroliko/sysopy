cc = gcc
cxx = g++
cflags = -std=gnu99 -pedantic -Wall -g
lflags = -Wall 
objs = list_ftw list_recursive common
main: ftw recursive
	
ftw: list_ftw.o common.o
	$(cc) $(cflags) -o $@ $^
recursive: list_recursive.o common.o
	$(cc) $(cflags) -o $@ $^
%.o: %.c 
	$(cc) $(cflags) -c $<	
clean:
	rm -f *.o *.a ftw recursive
