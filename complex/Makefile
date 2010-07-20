CC99=gcc -std=c99
INCLUDE=-I../include -I/usr/opt/include -L/usr/opt/lib
CFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith 		\
	-Wbad-function-cast -Wmissing-prototypes -Wstrict-prototypes 		\
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual	\
	-Wshadow -Wwrite-strings -Wno-unused-parameter				\
	-Wswitch -Wbad-function-cast -g $(INCLUDE)
CPPFLAGS=-Wall -W -g $(INCLUDE)
LIBS=-lm -latf-c

# For ../subr_*.c
VPATH = ../

obj/%.o: %.c
	mkdir -p obj
	$(CC99) -c -o $@ $< $(CFLAGS)

TESTS=	t_cacos t_cacosh t_carg t_casin t_casinh t_catan t_imgconstant

.PHONY: all
all: $(TESTS)

_DEPS_T_CACOS = t_cacos.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CACOS = $(_DEPS_T_CACOS:%=obj/%)
t_cacos: $(DEPS_T_CACOS)
	$(CC99) -o t_cacos $(DEPS_T_CACOS) $(CFLAGS) $(LIBS)


_DEPS_T_CACOSH = t_cacosh.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CACOSH = $(_DEPS_T_CACOSH:%=obj/%)
t_cacosh: $(DEPS_T_CACOSH)
	$(CC99) -o t_cacosh $(DEPS_T_CACOSH) $(CFLAGS) $(LIBS)


_DEPS_T_CARG = t_carg.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CARG = $(_DEPS_T_CARG:%=obj/%)
t_carg: $(DEPS_T_CARG)
	$(CC99) -o t_carg $(DEPS_T_CARG) $(CFLAGS) $(LIBS)


_DEPS_T_CASIN = t_casin.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CASIN = $(_DEPS_T_CASIN:%=obj/%)
t_casin: $(DEPS_T_CASIN)
	$(CC99) -o t_casin $(DEPS_T_CASIN) $(CFLAGS) $(LIBS)


_DEPS_T_CASINH = t_casinh.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CASINH = $(_DEPS_T_CASINH:%=obj/%)
t_casinh: $(DEPS_T_CASINH)
	$(CC99) -o t_casinh $(DEPS_T_CASINH) $(CFLAGS) $(LIBS)


_DEPS_T_CATAN = t_catan.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CATAN = $(_DEPS_T_CATAN:%=obj/%)
t_catan: $(DEPS_T_CATAN)
	$(CC99) -o t_catan $(DEPS_T_CATAN) $(CFLAGS) $(LIBS)


_DEPS_T_IMGCONSTANT = t_imgconstant.o subr_fpcmp.o
 DEPS_T_IMGCONSTANT = $(_DEPS_T_IMGCONSTANT:%=obj/%)
t_imgconstant: $(DEPS_T_IMGCONSTANT)
	$(CC99) -o t_imgconstant $(DEPS_T_IMGCONSTANT) $(CFLAGS) $(LIBS)


.PHONY: run
run:
	atf-run | atf-report

.PHONY: clean
clean:
	rm -f $(TESTS) obj/*.o *~ core

.PHONY: git-clean
git-clean:
	git clean -fd > /dev/null
