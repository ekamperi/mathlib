CC99=gcc -std=c99
CPP=g++
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

obj/%o: %cpp
	mkdir -p obj
	$(CPP) -c -o $@ $< $(CPPFLAGS)

TESTS=	t_extern t_feclearexcept t_fegetround t_fegetexceptflag t_fenv 		\
	t_feraiseexcept t_fesetenv t_fesetexceptflag t_ieeefp t_overrun

.PHONY: all
all: $(TESTS)

_DEPS_T_EXTERN = t_extern.o
 DEPS_T_EXTERN = $(_DEPS_T_EXTERN:%=obj/%)
t_extern: $(DEPS_T_EXTERN)
	$(CPP) -o t_extern $(DEPS_T_EXTERN) $(CPPFLAGS) -latf-c++ -latf-c


_DEPS_T_FECLEAREXCEPT = t_feclearexcept.o
 DEPS_T_FECLEAREXCEPT = $(_DEPS_T_FECLEAREXCEPT:%=obj/%)
t_feclearexcept: $(DEPS_T_FECLEAREXCEPT)
	$(CC99) -o t_feclearexcept $(DEPS_T_FECLEAREXCEPT) $(CFLAGS) $(LIBS)


_DEPS_T_FEGETROUND = t_fegetround.o
 DEPS_T_FEGETROUND = $(_DEPS_T_FEGETROUND:%=obj/%)
t_fegetround: $(DEPS_T_FEGETROUND)
	$(CC99) -o t_fegetround $(DEPS_T_FEGETROUND) $(CFLAGS) $(LIBS)


_DEPS_T_FEGETEXCEPTFLAG = t_fegetexceptflag.o subr_combinatorics.o
 DEPS_T_FEGETEXCEPTFLAG = $(_DEPS_T_FEGETEXCEPTFLAG:%=obj/%)
t_fegetexceptflag: $(DEPS_T_FEGETEXCEPTFLAG)
	$(CC99) -o t_fegetexceptflag $(DEPS_T_FEGETEXCEPTFLAG) $(CFLAGS) $(LIBS)


_DEPS_T_FEHOLDEXCEPT = t_feholdexcept.o
 DEPS_T_FEHOLDEXCEPT = $(_DEPS_T_FEHOLDEXCEPT:%=obj/%)
t_feholdexcept: $(DEPS_T_FEHOLDEXCEPT)
	$(CC99) -o t_feholdexcept $(DEPS_T_FEHOLDEXCEPT) $(CFLAGS) $(LIBS)


_DEPS_T_FENV = t_fenv.o
 DEPS_T_FENV = $(_DEPS_T_FENV:%=obj/%)
t_fenv: $(DEPS_T_FENV)
	$(CC99) -o t_feholdexcept $(DEPS_T_FENV) $(CFLAGS) $(LIBS)


_DEPS_T_FERAISEEXCEPT = t_feraiseexcept.o subr_combinatorics.o
 DEPS_T_FERAISEEXCEPT = $(_DEPS_T_FERAISEEXCEPT:%=obj/%)
t_feraiseexcept: $(DEPS_T_FERAISEEXCEPT)
	$(CC99) -o t_feraiseexcept $(DEPS_T_FERAISEEXCEPT) $(CFLAGS) $(LIBS)


_DEPS_T_FESETENV = t_fesetenv.o subr_combinatorics.o
 DEPS_T_FESETENV = $(_DEPS_T_FESETENV:%=obj/%)
t_fesetenv: $(DEPS_T_FESETENV)
	$(CC99) -o t_fesetenv $(DEPS_T_FESETENV) $(CFLAGS) $(LIBS)


_DEPS_T_FESETEXCEPTFLAG = t_fesetexceptflag.o subr_combinatorics.o
 DEPS_T_FESETEXCEPTFLAG = $(_DEPS_T_FESETEXCEPTFLAG:%=obj/%)
t_fesetexceptflag: $(DEPS_T_FESETEXCEPTFLAG)
	$(CC99) -o t_fesetexceptflag $(DEPS_T_FESETEXCEPTFLAG) $(CFLAGS) $(LIBS)


_DEPS_T_FESETROUND = t_fesetround.o
 DEPS_T_FESETROUND = $(_DEPS_T_FESETROUND:%=obj/%)
t_fesetround: $(DEPS_T_FESETROUND)
	$(CC99) -o t_fesetround $(DEPS_T_FESETROUND) $(CFLAGS) $(LIBS)


_DEPS_T_FEXCEPT = t_fexcept.o
 DEPS_T_FEXCEPT = $(_DEPS_T_FEXCEPT:%=obj/%)
t_fexcept: $(DEPS_T_FEXCEPT)
	$(CC99) -o t_fexcept $(DEPS_T_FEXCEPT) $(CFLAGS) $(LIBS)


_DEPS_T_FE_DFL_ENV = t_fe_dfl_env.o
 DEPS_T_FE_DFL_ENV = $(_DEPS_T_FE_DFL_ENV:%=obj/%)
t_fe_dfl_env: $(DEPS_T_FE_DFL_ENV)
	$(CC99) -o t_fe_dfl_env $(DEPS_T_FE_DFL_ENV) $(CFLAGS) $(LIBS)


_DEPS_T_IEEEFP = t_ieeefp.o
 DEPS_T_IEEEFP = $(_DEPS_T_IEEEFP:%=obj/%)
t_ieeefp: $(DEPS_T_IEEEFP)
	$(CC99) -o t_ieeefp $(DEPS_T_IEEEFP) $(CFLAGS) $(LIBS)


_DEPS_T_THREAD = t_thread.o
 DEPS_T_THREAD = $(_DEPS_T_THREAD:%=obj/%)
t_thread: $(DEPS_T_THREAD)
	$(CC99) -o t_thread $(DEPS_T_THREAD) $(CFLAGS) $(LIBS)


_DEPS_T_OVERRUN = t_overrun.o subr_atf.o
 DEPS_T_OVERRUN = $(_DEPS_T_OVERRUN:%=obj/%)
t_overrun: $(DEPS_T_OVERRUN)
	$(CC99) -o t_overrun $(DEPS_T_OVERRUN) $(CFLAGS) $(LIBS)


.PHONY: run
run:
	atf-run | atf-report

.PHONY: clean
clean:
	rm -f $(TESTS) obj/*.o *~ core

.PHONY: git-clean
git-clean:
	git clean -fd > /dev/null
