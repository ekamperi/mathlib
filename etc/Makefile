all:	goodgen proffenv testrand

# -Wstrict-prototypes
CC99=gcc -std=c99
CCFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith 		\
	-Wbad-function-cast -Wmissing-prototypes 				\
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual 	\
	-Wshadow -Wwrite-strings -Wno-unused-parameter 				\
	-Wswitch -Wbad-function-cast -g
INCLUDE=-I../include -I../ulps -I/usr/include/gmp -I/usr/include/mpfr
LIB=-lm -lmpfr

goodgen: goodgen.c ../ulps/gen.c ../subr_random.c
	@test -f ../ulps/config.h \
	|| (echo "You should first run autoreconf && ./configure in ../ulps" \
	&& exit 1)
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		goodgen.c ../ulps/gen.c ../subr_random.c	\
	-o goodgen						\
	$(LIB)

proffenv: proffenv.c
	$(CC99) $(CCFLAGS) $(INCLUDE) proffenv.c -o proffenv $(LIB)

profmath: profmath.c ../ulps/gen.c ../subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		profmath.c ../ulps/gen.c ../subr_random.c	\
	-o profmath $(LIB)

testrand: testrand.c ../subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		testrand.c ../subr_random.c			\
	-o testrand						\
	$(LIB)

gen-csv: profmath
	@./profmath

gen-graphs: gen-csv
	@mkdir -p graphs
	@rm -f graphs/*
	@./csv2plot.sh graphs/ 2>/dev/null

clean:
	rm -f obj/*.o *~ core *.csv

git-clean:
	git clean -f
