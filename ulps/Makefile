CC99=gcc -std=c99
# -Wstrict-prototypes
CCFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith \
	-Wbad-function-cast -Wmissing-prototypes \
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual \
	-Wshadow -Wwrite-strings -Wno-unused-parameter \
	-Wswitch -Wbad-function-cast -g
INCLUDE=-I../include -I/usr/include/gmp -I/usr/include/mpfr -I/usr/pkg/include
LIBS=-lm -lmpfr -L/usr/local/lib -L/usr/pkg/lib

# Don't use 'obj' or funny things will happen with VPATH set to ../
OBJDIR = ulpsobj

# For ../subr_*.c
VPATH=../

.PHONY: all
all:	ulpcalc

$(OBJDIR):
	mkdir -p $(OBJDIR)

config.h:
	autoreconf && ./configure

$(OBJDIR)/%.o: %.c | $(OBJDIR) config.h
	$(CC99) -c -o $@ $< $(CCFLAGS) $(INCLUDE)

_DEPS = main.o gen.o ulp.o subr_random.o
 DEPS = $(_DEPS:%=$(OBJDIR)/%)
ulpcalc: $(DEPS)
	$(CC99) -o ulps $(DEPS) $(CCFLAGS) $(INCLUDE) $(LIBS)

.PHONY: run
run:
	./ulps

# Hacky; create tar to ../ or else it will be included in the final tar
.PHONY: dist
dist:
	autoreconf && ./configure >/dev/null
	cp ../subr_random.c .
	cp ../include/subr_random.h .
	tar czfv ../ulp-0.1.tar.gz .
	mv ../ulp-0.1.tar.gz .

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core

.PHONY: git-clean
git-clean:
	git clean -fd > /dev/null
