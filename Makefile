CC99=gcc -std=c99
INCLUDE=-I./include -I/usr/opt/include -L/usr/opt/lib
CFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith             \
	-Wbad-function-cast -Wmissing-prototypes -Wstrict-prototypes         \
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual \
	-Wshadow -Wwrite-strings -Wno-unused-parameter                       \
	-Wswitch -Wbad-function-cast -g $(INCLUDE)
LIBS=-lm -latf-c


obj/%.o: %.c
	$(CC99) -c -o $@ $< $(CFLAGS)


all:	t_acos t_acosh t_asin t_asinh t_atan t_atan2 t_atanh t_cbrt t_ceil \
	t_constants t_copysign t_cos t_erf t_exp t_exp2 t_expm1 t_float    \
	t_floor t_fmax t_fpclassify t_fpmacros t_mac t_tgamma t_hypot      \
	t_ldexp t_nextafter t_signbit t_sin t_sqrt t_trig_ident t_tanh     \
	t_trunc t_types


DEPS_T_ACOS = obj/t_acos.o obj/subr_atf.o obj/subr_fpcmp.o
t_acos: $(DEPS_T_ACOS)
	$(CC99) -o t_acos $(DEPS_T_ACOS) $(CFLAGS) $(LIBS)


DEPS_T_ACOSH = obj/t_acosh.o obj/subr_fpcmp.o
t_acosh: $(DEPS_T_ACOSH)
	$(CC99) -o t_acosh $(DEPS_T_ACOSH) $(CFLAGS) $(LIBS)


DEPS_T_ASIN = obj/t_asin.o obj/subr_atf.o obj/subr_fpcmp.o
t_asin: $(DEPS_T_ASIN)
	$(CC99) -o t_asin $(DEPS_T_ASIN) $(CFLAGS) $(LIBS)


DEPS_T_ASINH = obj/t_asinh.o obj/subr_fpcmp.o
t_asinh: $(DEPS_T_ASINH)
	$(CC99) -o t_asinh $(DEPS_T_ASINH) $(CFLAGS) $(LIBS)


DEPS_T_ATAN = obj/t_atan.o obj/subr_atf.o obj/subr_fpcmp.o
t_atan:	$(DEPS_T_ATAN)
	$(CC99) -o t_atan $(DEPS_T_ATAN) $(CFLAGS) $(LIBS)


DEPS_T_ATAN2 = obj/t_atan2.o obj/subr_atf.o obj/subr_fpcmp.o
t_atan2: $(DEPS_T_ATAN2)
	$(CC99) -o t_atan2 $(DEPS_T_ATAN2) $(CFLAGS) $(LIBS)


DEPS_T_ATANH = obj/t_atanh.o obj/subr_atf.o obj/subr_errhandling.o obj/subr_fpcmp.o
t_atanh: $(DEPS_T_ATANH)
	$(CC99) -o t_atanh $(DEPS_T_ATANH) $(CFLAGS) $(LIBS)


DEPS_T_CBRT = obj/t_cbrt.o obj/subr_atf.o obj/subr_fpcmp.o
t_cbrt: $(DEPS_T_CBRT)
	$(CC99) -o t_cbrt $(DEPS_T_CBRT) $(CFLAGS) $(LIBS)


DEPS_T_CEIL = obj/t_ceil.o obj/subr_atf.o obj/subr_fpcmp.o obj/subr_random.o
t_ceil: $(DEPS_T_CEIL)
	$(CC99) -o t_ceil $(DEPS_T_CEIL) $(CFLAGS) $(LIBS)


DEPS_T_CONSTANTS = obj/t_constants.o
t_constants: $(DEPS_T_CONSTANTS)
	$(CC99) -o t_constants $(DEPS_T_CONSTANTS) $(CFLAGS) $(LIBS)


DEPS_T_COPYSIGN = obj/t_copysign.o obj/subr_atf.o obj/subr_random.o
t_copysign: $(DEPS_T_COPYSIGN)
	$(CC99) -o t_copysign $(DEPS_T_COPYSIGN) $(CFLAGS) $(LIBS)


DEPS_T_COS = obj/t_cos.o obj/subr_atf.o obj/subr_errhandling.o	\
	     obj/subr_fpcmp.o obj/subr_random.o
t_cos: $(DEPS_T_COS)
	$(CC99) -o t_cos $(DEPS_T_COS) $(CFLAGS) $(LIBS)


DEPS_T_ERF = obj/t_erf.o obj/subr_atf.o obj/subr_errhandling.o  \
             obj/subr_fpcmp.o obj/subr_random.o
t_erf: $(DEPS_T_ERF)
	$(CC99) -o t_erf $(DEPS_T_ERF) $(CFLAGS) $(LIBS)


.PHONY: clean
clean:
	rm -f obj/*.o *~ core
