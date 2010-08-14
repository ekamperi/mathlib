CC99=gcc -std=c99
INCLUDE=-I../include -I/usr/opt/include -L/usr/opt/lib
CFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith 		\
	-Wbad-function-cast -Wmissing-prototypes -Wstrict-prototypes 		\
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual	\
	-Wshadow -Wwrite-strings -Wno-unused-parameter				\
	-Wswitch -Wbad-function-cast -g $(INCLUDE)
CPPFLAGS=-Wall -W -g $(INCLUDE)
LIBS=-lm -latf-c

TESTS=  t_cabs t_cacos t_cacosh t_carg t_casin t_casinh t_catan t_catanh	\
	t_cimag t_clog t_conj t_creal t_csqrt t_imgconstant

.PHONY: all
all: $(TESTS)

# For ../subr_*.c
VPATH = ../

# Don't use 'obj' or funny things will happen with VPATH set to ../
OBJDIR = obj-complex

$(OBJDIR):
	mkdir -p $(OBJDIR)

include/config.h:
	autoreconf ../ && ../configure
	@mv include/config.h ../include/config.h
	@rm -rf include

$(OBJDIR)/%.o: %.c | $(OBJDIR) include/config.h
	$(CC99) -c -o $@ $< $(CFLAGS)


_DEPS_T_CABS = t_cabs.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CABS = $(_DEPS_T_CABS:%=$(OBJDIR)/%)
t_cabs: $(DEPS_T_CABS)
	$(CC99) -o t_cabs $(DEPS_T_CABS) $(CFLAGS) $(LIBS)

_DEPS_T_CACOS = t_cacos.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CACOS = $(_DEPS_T_CACOS:%=$(OBJDIR)/%)
t_cacos: $(DEPS_T_CACOS)
	$(CC99) -o t_cacos $(DEPS_T_CACOS) $(CFLAGS) $(LIBS)


_DEPS_T_CACOSH = t_cacosh.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CACOSH = $(_DEPS_T_CACOSH:%=$(OBJDIR)/%)
t_cacosh: $(DEPS_T_CACOSH)
	$(CC99) -o t_cacosh $(DEPS_T_CACOSH) $(CFLAGS) $(LIBS)


_DEPS_T_CARG = t_carg.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CARG = $(_DEPS_T_CARG:%=$(OBJDIR)/%)
t_carg: $(DEPS_T_CARG)
	$(CC99) -o t_carg $(DEPS_T_CARG) $(CFLAGS) $(LIBS)


_DEPS_T_CASIN = t_casin.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CASIN = $(_DEPS_T_CASIN:%=$(OBJDIR)/%)
t_casin: $(DEPS_T_CASIN)
	$(CC99) -o t_casin $(DEPS_T_CASIN) $(CFLAGS) $(LIBS)


_DEPS_T_CASINH = t_casinh.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CASINH = $(_DEPS_T_CASINH:%=$(OBJDIR)/%)
t_casinh: $(DEPS_T_CASINH)
	$(CC99) -o t_casinh $(DEPS_T_CASINH) $(CFLAGS) $(LIBS)


_DEPS_T_CATAN = t_catan.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CATAN = $(_DEPS_T_CATAN:%=$(OBJDIR)/%)
t_catan: $(DEPS_T_CATAN)
	$(CC99) -o t_catan $(DEPS_T_CATAN) $(CFLAGS) $(LIBS)


_DEPS_T_CATANH = t_catanh.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CATANH = $(_DEPS_T_CATANH:%=$(OBJDIR)/%)
t_catanh: $(DEPS_T_CATANH)
	$(CC99) -o t_catanh $(DEPS_T_CATANH) $(CFLAGS) $(LIBS)


_DEPS_T_CIMAG = t_cimag.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CIMAG = $(_DEPS_T_CIMAG:%=$(OBJDIR)/%)
t_cimag: $(DEPS_T_CIMAG)
	$(CC99) -o t_cimag $(DEPS_T_CIMAG) $(CFLAGS) $(LIBS)


_DEPS_T_CLOG = t_clog.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CLOG = $(_DEPS_T_CLOG:%=$(OBJDIR)/%)
t_clog: $(DEPS_T_CLOG)
	$(CC99) -o t_clog $(DEPS_T_CLOG) $(CFLAGS) $(LIBS)


_DEPS_T_CONJ = t_conj.o subr_atf.o subr_random.o
 DEPS_T_CONJ = $(_DEPS_T_CONJ:%=$(OBJDIR)/%)
t_conj: $(DEPS_T_CONJ)
	$(CC99) -o t_conj $(DEPS_T_CONJ) $(CFLAGS) $(LIBS)


_DEPS_T_CREAL = t_creal.o subr_atf.o subr_random.o
 DEPS_T_CREAL = $(_DEPS_T_CREAL:%=$(OBJDIR)/%)
t_creal: $(DEPS_T_CREAL)
	$(CC99) -o t_creal $(DEPS_T_CREAL) $(CFLAGS) $(LIBS)

_DEPS_T_CSQRT = t_csqrt.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CSQRT = $(_DEPS_T_CSQRT:%=$(OBJDIR)/%)
t_csqrt: $(DEPS_T_CSQRT)
	$(CC99) -o t_csqrt $(DEPS_T_CSQRT) $(CFLAGS) $(LIBS)


_DEPS_T_IMGCONSTANT = t_imgconstant.o subr_fpcmp.o
 DEPS_T_IMGCONSTANT = $(_DEPS_T_IMGCONSTANT:%=$(OBJDIR)/%)
t_imgconstant: $(DEPS_T_IMGCONSTANT)
	$(CC99) -o t_imgconstant $(DEPS_T_IMGCONSTANT) $(CFLAGS) $(LIBS)


.PHONY: run
run:
	atf-run | atf-report

.PHONY: clean
clean:
	rm -f $(TESTS) $(OBJDIR)/*.o *~ core

.PHONY: git-clean
git-clean:
	git clean -fd > /dev/null
