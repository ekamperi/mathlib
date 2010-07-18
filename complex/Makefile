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

TESTS=	t_cacos t_cacosh t_carg t_imgconstant

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
