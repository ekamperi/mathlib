PROGS=proffenv profmath realgen testrand testrand2
ALL: $(PROGS)

# -Wstrict-prototypes
CC99=gcc -std=c99
CCFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith 		\
	-Wbad-function-cast -Wmissing-prototypes 				\
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual 	\
	-Wshadow -Wwrite-strings -Wno-unused-parameter 				\
	-Wswitch -Wbad-function-cast -g
INCLUDE=-I../include -I../ulps -I/usr/include/gmp -I/usr/include/mpfr		\
	-I/usr/opt/include -I/usr/local/include
LIB=-lm -lmpfr -lmpc -L/usr/opt/lib -L/usr/local/lib -R/usr/opt/lib

proffenv: proffenv.c
	$(CC99) $(CCFLAGS) $(INCLUDE) proffenv.c -o proffenv $(LIB)

profmath: profmath.c ../ulps/basic-complex.c ../ulps/gen.c ../subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		profmath.c ../ulps/basic-complex.c ../ulps/gen.c ../subr_random.c	\
	-o profmath $(LIB)

realgen: realgen.c ../ulps/basic-complex.c ../ulps/gen.c ../subr_random.c
	@test -f ../ulps/config.h \
	|| (echo "You should first run autoreconf && ./configure in ../ulps" \
	&& exit 1)
	$(CC99) $(CCFLAGS) $(INCLUDE)                           \
		realgen.c ../ulps/basic-complex.c ../ulps/gen.c \
		../subr_random.c        			\
	-o realgen                                              \
	$(LIB)

testrand: testrand.c ../subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		testrand.c ../subr_random.c			\
	-o testrand -lm

testrand2: testrand2.c ../subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		testrand2.c ../subr_random.c			\
	-o testrand2 -lm

gen-csv: profmath

gen-graphs: gen-csv
	@type gnuplot > /dev/null || \
	(echo "Sorry, you need to have gnuplot installed" && exit 111)
	@echo "*** Generating .csv data files ***"
	@./profmath
	@echo "*** Generating .png plot graphs ***"
	@mkdir -p graphs
	@rm -f graphs/*
	@./csv2plot.sh graphs
	@mkdir -p csv
	@rm -f csv/*
	@mv *.csv csv/

clean:
	rm -f $(PROGS) *.o *~ core

git-clean:
	git clean -fd > /dev/null
