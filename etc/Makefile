all:	testrand

CC99=gcc -std=c99
CCFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith 		\
	-Wbad-function-cast -Wmissing-prototypes -Wstrict-prototypes 		\
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual 	\
	-Wshadow -Wwrite-strings -Wno-unused-parameter 				\
	-Wswitch -Wbad-function-cast -g
LIB=-lm

testrand: testrand.c ../subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		testrand.c ../subr_random.c			\
	-o testrand						\
	$(LIB)

clean:
	git clean -f
