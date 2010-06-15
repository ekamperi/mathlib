CC99=gcc -std=c99
INCLUDE=-I./include -I/usr/opt/include -L/usr/opt/lib
CFLAGS=-Wall -g $(INCLUDE)
LIBS=-lm -latf-c

obj/%.o: %.c
	$(CC99) -c -o $@ $< $(CFLAGS)


all:	t_acos t_acosh t_asin t_asinh t_atan t_atan2 t_atanh t_cbrt t_ceil \
	t_constants t_copysign t_cos t_erf t_exp t_exp2 t_expm1 t_float    \
	t_floor t_fmax t_fpclassify t_fpmacros t_mac t_tgamma t_hypot      \
	t_ldexp t_nextafter t_signbit t_sin t_sqrt t_trig_ident t_tanh     \
	t_trunc t_types

_DEPS_T_ACOS = t_acos.o subr_atf.o subr_fpcmp.o
 DEPS_T_ACOS = $(_DEPS_T_ACOS:%=obj/%)
t_acos: $(DEPS_T_ACOS)
	$(CC99) -o t_acos $(DEPS_T_ACOS) $(CFLAGS) $(LIBS)


_DEPS_T_ACOSH = t_acosh.o subr_fpcmp.o
 DEPS_T_ACOSH = $(_DEPS_T_ACOSH:%=obj/%)
t_acosh: $(DEPS_T_ACOSH)
	$(CC99) -o t_acosh $(DEPS_T_ACOSH) $(CFLAGS) $(LIBS)


_DEPS_T_ASIN = t_asin.o subr_atf.o subr_fpcmp.o
 DEPS_T_ASIN = $(_DEPS_T_ASIN:%=obj/%)
t_asin: $(DEPS_T_ASIN)
	$(CC99) -o t_asin $(DEPS_T_ASIN) $(CFLAGS) $(LIBS)


_DEPS_T_ASINH = t_asinh.o subr_fpcmp.o
 DEPS_T_ASINH = $(_DEPS_T_ASINH:%=obj/%)
t_asinh: $(DEPS_T_ASINH)
	$(CC99) -o t_asinh $(DEPS_T_ASINH) $(CFLAGS) $(LIBS)


_DEPS_T_ATAN = t_atan.o subr_atf.o subr_fpcmp.o
 DEPS_T_ATAN = $(_DEPS_T_ATAN:%=obj/%)
t_atan:	$(DEPS_T_ATAN)
	$(CC99) -o t_atan $(DEPS_T_ATAN) $(CFLAGS) $(LIBS)


_DEPS_T_ATAN2 = t_atan2.o subr_atf.o subr_fpcmp.o
 DEPS_T_ATAN2 = $(_DEPS_T_ATAN2:%=obj/%)
t_atan2: $(DEPS_T_ATAN2)
	$(CC99) -o t_atan2 $(DEPS_T_ATAN2) $(CFLAGS) $(LIBS)


_DEPS_T_ATANH = t_atanh.o subr_atf.o subr_errhandling.o subr_fpcmp.o
 DEPS_T_ATANH = $(_DEPS_T_ATANH:%=obj/%)
t_atanh: $(DEPS_T_ATANH)
	$(CC99) -o t_atanh $(DEPS_T_ATANH) $(CFLAGS) $(LIBS)


_DEPS_T_CBRT = t_cbrt.o subr_atf.o subr_fpcmp.o
 DEPS_T_CBRT = $(_DEPS_T_CBRT:%=obj/%)
t_cbrt: $(DEPS_T_CBRT)
	$(CC99) -o t_cbrt $(DEPS_T_CBRT) $(CFLAGS) $(LIBS)


_DEPS_T_CEIL = t_ceil.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CEIL= $(_DEPS_T_CEIL:%=obj/%)
t_ceil: $(DEPS_T_CEIL)
	$(CC99) -o t_ceil $(DEPS_T_CEIL) $(CFLAGS) $(LIBS)


_DEPS_T_CONSTANTS = t_constants.o
 DEPS_T_CONSTANTS = $(_DEPS_T_CONSTANTS:%=obj/%)
t_constants: $(DEPS_T_CONSTANTS)
	$(CC99) -o t_constants $(DEPS_T_CONSTANTS) $(CFLAGS) $(LIBS)


_DEPS_T_COPYSIGN = t_copysign.o subr_atf.o subr_random.o
 DEPS_T_COPYSIGNH = $(_DEPS_T_COPYSIGN:%=obj/%)
t_copysign: $(DEPS_T_COPYSIGN)
	$(CC99) -o t_copysign $(DEPS_T_COPYSIGN) $(CFLAGS) $(LIBS)


_DEPS_T_COS = t_cos.o subr_atf.o subr_errhandling.o	\
	     subr_fpcmp.o subr_random.o
 DEPS_T_COS = $(_DEPS_T_COS:%=obj/%)
t_cos: $(DEPS_T_COS)
	$(CC99) -o t_cos $(DEPS_T_COS) $(CFLAGS) $(LIBS)


_DEPS_T_ERF = t_erf.o subr_atf.o subr_errhandling.o  \
             subr_fpcmp.o subr_random.o
 DEPS_T_ERF = $(_DEPS_T_ERF:%=obj/%)
t_erf: $(DEPS_T_ERF)
	$(CC99) -o t_erf $(DEPS_T_ERF) $(CFLAGS) $(LIBS)


_DEPS_T_EXP = t_exp.o subr_atf.o subr_errhandling.o  \
	      subr_fpcmp.o subr_random.o
 DEPS_T_EXP = $(_DEPS_T_EXP:%=obj/%)
t_exp: $(DEPS_T_EXP)
	$(CC99) -o t_erf $(DEPS_T_EXP) $(CFLAGS) $(LIBS)


_DEPS_T_EXP2 = t_exp2.o subr_atf.o subr_errhandling.o  \
	       subr_fpcmp.o subr_random.o
 DEPS_T_EXP2 = $(_DEPS_T_EXP2:%=obj/%)
t_exp2: $(DEPS_T_EXP2)
	$(CC99) -o t_exp2 $(DEPS_T_EXP2) $(CFLAGS) $(LIBS)


_DEPS_T_EXPM1 = t_expm1.o subr_atf.o subr_errhandling.o  \
		subr_fpcmp.o subr_random.o
 DEPS_T_EXPM1 = $(_DEPS_T_EXPM1:%=obj/%)
t_expm1: $(DEPS_T_EXPM1)
	$(CC99) -o t_erf $(DEPS_T_EXPM1) $(CFLAGS) $(LIBS)


_DEPS_T_FMAX = t_fmax.o subr_atf.o subr_random.o
 DEPS_T_FMAX = $(_DEPS_T_FMAX:%=obj/%)
t_fmax: $(DEPS_T_FMAX)
	$(CC99) -o t_fmax $(DEPS_T_FMAX) $(CFLAGS) $(LIBS)

_DEPS_T_FLOAT = t_float.o
 DEPS_T_FLOAT = $(_DEPS_T_FLOAT:%=obj/%)
t_float: $(DEPS_T_FLOAT)
	$(CC99) -o t_float $(DEPS_T_FLOAT) $(CFLAGS) $(LIBS)


.PHONY: clean
clean:
	rm -f obj/*.o *~ core
