all:	t_feclearexcept		\
	t_fegetround		\
	t_fegetexceptflag	\
	t_feholdexcept		\
	t_fenv			\
	t_feraiseexcept		\
	t_fesetenv		\
	t_fesetexceptflag	\
	t_fesetround		\
	t_fexcept		\
	t_fe_dfl_env		\

CC99=gcc -std=c99
CCFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith 		\
	-Wbad-function-cast -Wmissing-prototypes -Wstrict-prototypes 		\
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual 	\
	-Wshadow -Wwrite-strings -Wno-unused-parameter 				\
	-Wswitch -Wbad-function-cast -g
INCLUDE=-I/usr/opt/include
LIB=-L/usr/opt/lib -lm -latf-c

t_feclearexcept: t_feclearexcept.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_feclearexcept.c				\
	-o t_feclearexcept					\
	$(LIB)

t_fegetround: t_fegetround.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_fegetround.c					\
	-o t_fegetround						\
	$(LIB)

t_fegetexceptflag: t_fegetexceptflag.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_fegetexceptflag.c ../subr_combinatorics.c	\
	-o t_fegetexceptflag					\
	$(LIB)

t_feholdexcept: t_feholdexcept.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_feholdexcept.c				\
	-o t_feholdexcept					\
	$(LIB)

t_fenv: t_fenv.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_fenv.c					\
	-o t_fenv						\
	 $(LIB)

t_feraiseexcept: t_feraiseexcept.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_feraiseexcept.c ../subr_combinatorics.c	\
	-o t_feraiseexcept					\
	$(LIB)

t_fesetenv: t_fesetenv.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_fesetenv.c					\
	-o t_fesetenv						\
	$(LIB)

t_fesetexceptflag: t_fesetexceptflag.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
	t_fesetexceptflag.c ../subr_combinatorics.c		\
		-o t_fesetexceptflag				\
	$(LIB)

t_fesetround: t_fesetround.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_fesetround.c					\
	-o t_fesetround						\
	$(LIB)

t_fexcept: t_fexcept.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_fexcept.c					\
	-o t_fexcept						\
	$(LIB)

t_fe_dfl_env: t_fe_dfl_env.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_fe_dfl_env.c					\
	-o t_fe_dfl_env						\
	$(LIB)

clean:
	git clean -f

run:
	atf-run | atf-report
