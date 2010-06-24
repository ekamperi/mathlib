CC99=gcc -std=c99
# -Wstrict-prototypes
CCFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith \
	-Wbad-function-cast -Wmissing-prototypes \
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual \
	-Wshadow -Wwrite-strings -Wno-unused-parameter \
	-Wswitch -Wbad-function-cast -g
INCLUDE=-I../include -I/usr/include/gmp -I/usr/include/mpfr -I/usr/pkg/include
LIBS=-lm -lmpfr -L/usr/local/lib -L/usr/pkg/lib

VPATH = ../
SOURCES=main.c gen.c ulp.c subr_random.c

all: $(SOURCES)
	@test -f config.h || \
		(echo "You need to run autoreconf && ./configure first"; exit 1)
	$(CC99) $(SOURCES) -o ulps $(CCFLAGS) $(INCLUDE) $(LIBS)

# Hacky; create tar to ../ or else it will be included in the final tar
dist:
	@tar czfv ../ulp-0.1.tar.gz \
		  ../subr_random.c ../include/subr_random.h .
	@mv ../ulp-0.1.tar.gz .

git-clean:
	git clean -f -d
