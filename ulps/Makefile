CC99=gcc -std=c99
# -Wstrict-prototypes
CCFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith \
	-Wbad-function-cast -Wmissing-prototypes \
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual \
	-Wshadow -Wwrite-strings -Wno-unused-parameter \
	-Wswitch -Wbad-function-cast -g
INCLUDE=-I../include -I/usr/include/gmp -I/usr/include/mpfr
LIB=-lm -lmpfr

SOURCES=main.c gen.c ulp.c ../subr_random.c

all: $(SOURCES)
	@test -f config.h || \
		(echo "You need to run autoreconf && ./configure first"; exit 1)
	$(CC99) $(SOURCES) -o ulps $(CCFLAGS) $(INCLUDE) $(LIB)
