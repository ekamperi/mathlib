all:	t_acos		\
	t_acosh		\
	t_asin		\
	t_asinh		\
	t_atan		\
	t_atan2		\
	t_atanh		\
	t_cbrt		\
	t_ceil		\
	t_constants	\
	t_copysign	\
	t_float		\
	t_floor		\
	t_fmax		\
	t_fpclassify	\
	t_fpmacros	\
	t_mac		\
	t_tgamma	\
	t_hypot		\
	t_ldexp		\
	t_nextafter	\
	t_sqrt		\
	t_trig_ident	\
	t_tanh		\
	t_trunc		\
	t_types		\

CC99=gcc -std=c99
CCFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith 		\
	-Wbad-function-cast -Wmissing-prototypes -Wstrict-prototypes 		\
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual 	\
	-Wshadow -Wwrite-strings -Wno-unused-parameter 				\
	-Wswitch -Wbad-function-cast -g
INCLUDE=-I/usr/opt/include
LIB=-L/usr/opt/lib -lm -latf-c

foo: foo.c subr_atf.c
	$(CC99) $(CCFLAGS) $(INCLUDE) foo.c subr_atf.c -o foo $(LIB)

t_acos: t_acos.c subr_atf.c subr_fpcmp.h
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_acos.c subr_atf.c				\
	-o t_acos 						\
	$(LIB)

t_acosh: t_acosh.c subr_fpcmp.h
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_acosh.c					\
	-o t_acosh						\
	$(LIB)

t_asin: t_asin.c subr_fpcmp.h
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_asin.c 					\
	-o t_asin 						\
	$(LIB)

t_asinh: t_asinh.c subr_fpcmp.h
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_asinh.c					\
	-o t_asinh						\
	$(LIB)

t_atan: t_atan.c subr_fpcmp.h
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_atan.c					\
	-o t_atan						\
	$(LIB)

t_atan2: t_atan2.c
	$(CC99) $(INCLUDE)					\
		t_atan2.c					\
	-o t_atan2						\
	$(LIB)

t_atanh: t_atanh.c subr_errhandling.c subr_fpcmp.h
	$(CC99) $(INCLUDE)					\
		t_atanh.c subr_errhandling.c			\
	-o t_atanh						\
	$(LIB)

t_cbrt: t_cbrt.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_cbrt.c					\
	-o t_cbrt						\
	 $(LIB)

t_ceil: t_ceil.c subr_atf.c subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_ceil.c subr_atf.c subr_random.c		\
	-o t_ceil						\
	 $(LIB)

t_constants: t_constants.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_constants.c					\
	-o t_constants						\
	$(LIB)

t_copysign: t_copysign.c subr_atf.c subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_copysign.c subr_atf.c subr_random.c		\
	-o t_copysign						\
	$(LIB)

t_fmax: t_fmax.c subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_fmax.c subr_random.c				\
	-o t_fmax 						\
	$(LIB)

t_float: t_float.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_float.c					\
	-o t_float 						\
	$(LIB)

t_floor: t_floor.c subr_atf.c subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_floor.c subr_atf.c subr_random.c		\
	-o t_floor						\
	$(LIB)

t_fpclassify: t_fpclassify.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_fpclassify.c					\
	-o t_fpclassify			 			\
	$(LIB)

t_fpmacros: t_fpmacros.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_fpmacros.c					\
	-o t_fpmacros 						\
	$(LIB)

t_hypot: t_hypot.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_hypot.c					\
	-o t_hypot		 				\
	$(LIB)

t_ldexp: t_ldexp.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_ldexp.c					\
	-o t_ldexp 						\
	$(LIB)

t_mac: t_mac.c subr_atf.c subr_random.c subr_errhandling.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_mac.c subr_atf.c subr_random.c subr_errhandling.c 	\
	-o t_mac						\
	$(LIB)

t_nextafter: t_nextafter.c subr_random.c subr_errhandling.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_nextafter.c subr_errhandling.c subr_random.c	\
	 -o t_nextafter 					\
	$(LIB)

t_sqrt: t_sqrt.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_sqrt.c					\
	-o t_sqrt						\
	$(LIB)

t_tanh: t_tanh.c subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_tanh.c subr_random.c				\
	-o t_tanh						\
	 $(LIB)

t_tgamma: t_tgamma.c subr_random.c subr_errhandling.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_tgamma.c subr_random.c subr_errhandling.c	\
	-o t_tgamma						\
	$(LIB)

t_trig_ident: t_trig_ident.c subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_trig_ident.c subr_random.c			\
	-o t_trig_ident						\
	$(LIB)

t_trunc: t_trunc.c subr_atf.c subr_random.c
	$(CC99) $(CCFLAGS) $(INCLUDE)				\
		t_trunc.c subr_atf.c subr_random.c 		\
	-o t_trunc						\
	$(LIB)

t_types: t_types.c
	$(CC99) $(CCFLAGS) $(INCLUDE) 				\
		t_types.c					\
	-o t_types						\
	$(LIB)

clean:
	git clean -f

run:
	atf-run . fenv | atf-report

run-xml:
	rm -f tmp.xml tmp2.xml tmp3.xml
	rm -f tmp.html tmp2.html
	atf-run . fenv | atf-report -o xml:tmp.xml
	cat tmp.xml | col > tmp2.xml
	sed 's/http:\/\/www.NetBSD.org\/XML\/atf/\./' tmp2.xml > tmp3.xml
	xsltproc ./tests-results.xsl tmp3.xml > tmp.html
	sed 's/tests-results.css/\.\.\/\.\.\/\.\.\/\.\.\/css\/tests-results.css/' tmp.html > tmp2.html
